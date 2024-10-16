#include "memory.h"

#define ALIGN4(size) (((size) + 3) & ~3)
#define HEAP_MAGIC 0xDEADBEEF

typedef struct block {
    size_t size;
    struct block* next;
    int free;
    unsigned int magic;
} block_t;

static block_t* free_list = NULL;

void memory_init(void* heap_start, size_t heap_size) {
    free_list = (block_t*)heap_start;
    free_list->size = heap_size - sizeof(block_t);
    free_list->next = NULL;
    free_list->free = 1;
    free_list->magic = HEAP_MAGIC;
}

void* malloc(size_t size) {
    block_t* current = free_list;
    block_t* prev = NULL;
    size_t aligned_size = ALIGN4(size);

    while (current && !(current->free && current->size >= aligned_size)) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        return NULL; // No suitable block found
    }

    if (current->size > aligned_size + sizeof(block_t)) {
        block_t* new_block = (block_t*)((char*)current + sizeof(block_t) + aligned_size);
        new_block->size = current->size - aligned_size - sizeof(block_t);
        new_block->next = current->next;
        new_block->free = 1;
        new_block->magic = HEAP_MAGIC;

        current->size = aligned_size;
        current->next = new_block;
    }

    current->free = 0;
    return (void*)((char*)current + sizeof(block_t));
}

void free(void* ptr) {
    if (!ptr) {
        return;
    }

    block_t* block = (block_t*)((char*)ptr - sizeof(block_t));
    if (block->magic != HEAP_MAGIC) {
        return; // Invalid block
    }

    block->free = 1;

    // Coalesce adjacent free blocks
    block_t* current = free_list;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += current->next->size + sizeof(block_t);
            current->next = current->next->next;
        }
        current = current->next;
    }
}