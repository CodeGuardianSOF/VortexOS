#include "memory.h"
#include <stddef.h>
#include <stdint.h>
#include "vga.h"

#define ALIGN8(size) (((size) + 7) & ~7)
#define HEAP_MAGIC 0xDEADBEEF
#define NUM_FREE_LISTS 10
#define POOL_SIZE 1024

typedef struct block {
    size_t size;
    struct block* next;
    struct block* prev;
    int free;
    unsigned int magic;
} block_t;

typedef struct {
    volatile int lock;
} simple_mutex_t;

static block_t* free_lists[NUM_FREE_LISTS] = {NULL};
static block_t* memory_pools[NUM_FREE_LISTS] = {NULL};
static simple_mutex_t memory_mutex = {0};

static size_t get_free_list_index(size_t size) {
    size_t index = 0;
    size >>= 3;
    while (size > 1 && index < NUM_FREE_LISTS - 1) {
        size >>= 1;
        index++;
    }
    return index;
}

static void simple_mutex_lock(simple_mutex_t* mutex) {
    while (__sync_lock_test_and_set(&mutex->lock, 1)) {
        // Busy-wait
    }
}

static void simple_mutex_unlock(simple_mutex_t* mutex) {
    __sync_lock_release(&mutex->lock);
}

void memory_init(void* heap_start, size_t heap_size) {
    simple_mutex_lock(&memory_mutex);

    block_t* initial_block = (block_t*)heap_start;
    initial_block->size = heap_size - sizeof(block_t);
    initial_block->next = NULL;
    initial_block->prev = NULL;
    initial_block->free = 1;
    initial_block->magic = HEAP_MAGIC;

    size_t index = get_free_list_index(initial_block->size);
    free_lists[index] = initial_block;

    simple_mutex_unlock(&memory_mutex);
}

void* malloc(size_t size) {
    simple_mutex_lock(&memory_mutex);

    size_t aligned_size = ALIGN8(size);
    size_t index = get_free_list_index(aligned_size);

    for (; index < NUM_FREE_LISTS; index++) {
        block_t* current = free_lists[index];
        while (current && !(current->free && current->size >= aligned_size)) {
            current = current->next;
        }

        if (current) {
            if (current->size > aligned_size + sizeof(block_t)) {
                block_t* new_block = (block_t*)((char*)current + sizeof(block_t) + aligned_size);
                new_block->size = current->size - aligned_size - sizeof(block_t);
                new_block->next = current->next;
                new_block->prev = current;
                new_block->free = 1;
                new_block->magic = HEAP_MAGIC;

                current->size = aligned_size;
                current->next = new_block;
                if (new_block->next) {
                    new_block->next->prev = new_block;
                }

                size_t new_index = get_free_list_index(new_block->size);
                new_block->next = free_lists[new_index];
                if (free_lists[new_index]) {
                    free_lists[new_index]->prev = new_block;
                }
                free_lists[new_index] = new_block;
            }

            current->free = 0;
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                free_lists[index] = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }

            simple_mutex_unlock(&memory_mutex);
            return (void*)((char*)current + sizeof(block_t));
        }
    }

    simple_mutex_unlock(&memory_mutex);
    return NULL; // No suitable block found
}

void free(void* ptr) {
    if (!ptr) {
        return;
    }

    simple_mutex_lock(&memory_mutex);

    block_t* block = (block_t*)((char*)ptr - sizeof(block_t));
    if (block->magic != HEAP_MAGIC) {
        simple_mutex_unlock(&memory_mutex);
        return; // Invalid block
    }

    block->free = 1;

    // Coalesce with next block if possible
    if (block->next && block->next->free) {
        block->size += block->next->size + sizeof(block_t);
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }

    // Coalesce with previous block if possible
    if (block->prev && block->prev->free) {
        block->prev->size += block->size + sizeof(block_t);
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
        block = block->prev;
    }

    size_t index = get_free_list_index(block->size);

    // Insert the block into the appropriate free list
    block->next = free_lists[index];
    block->prev = NULL;

    if (free_lists[index]) {
        free_lists[index]->prev = block;
    }

    free_lists[index] = block;

    simple_mutex_unlock(&memory_mutex);
}

void defragment_memory() {
    simple_mutex_lock(&memory_mutex);

    for (size_t i = 0; i < NUM_FREE_LISTS; i++) {
        block_t* current = free_lists[i];
        while (current) {
            block_t* next = current->next;
            while (next && next->free) {
                current->size += next->size + sizeof(block_t);
                current->next = next->next;
                if (next->next) {
                    next->next->prev = current;
                }
                next = current->next;
            }
            current = current->next;
        }
    }

    simple_mutex_unlock(&memory_mutex);
}