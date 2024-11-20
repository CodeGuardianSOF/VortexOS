#include "memory.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "snprintf.h"
#include "vga.h"

extern char __memory_pool_start;
extern char __memory_pool_end;

#define ALIGNMENT 8
#define ALIGN_MASK (ALIGNMENT - 1)

#define ERR_NO_MEMORY 1
#define KMALLOC_FLAG_RESERVED 0x1

typedef struct MemoryBlock {
    size_t size;
    bool is_free;
    struct MemoryBlock* next;
    struct MemoryBlock* prev;
} MemoryBlock;

static char* memory_pool_start;
static size_t memory_pool_size;
static MemoryBlock* free_list = NULL;
static MemoryBlock* allocated_list = NULL;
static MemoryBlock* reserved_free_list = NULL;
static MemoryBlock* reserved_allocated_list = NULL;

static int last_error;

static size_t align_size(size_t size) {
    return (size + ALIGN_MASK) & ~ALIGN_MASK;
}

void initialize_memory_manager() {
    memory_pool_start = &__memory_pool_start;
    memory_pool_size = (uintptr_t)&__memory_pool_end - (uintptr_t)&__memory_pool_start;

    uintptr_t reserved_size = memory_pool_size / 4;
    uintptr_t general_size = memory_pool_size - reserved_size;

    // Initialize general pool
    free_list = (MemoryBlock*)memory_pool_start;
    free_list->size = general_size - sizeof(MemoryBlock);
    free_list->is_free = true;
    free_list->next = NULL;
    free_list->prev = NULL;

    // Initialize reserved pool
    reserved_free_list = (MemoryBlock*)(memory_pool_start + general_size);
    reserved_free_list->size = reserved_size - sizeof(MemoryBlock);
    reserved_free_list->is_free = true;
    reserved_free_list->next = NULL;
    reserved_free_list->prev = NULL;
}


static void split_block(MemoryBlock* block, size_t size) {
    if (block->size >= size + sizeof(MemoryBlock) + ALIGNMENT) {
        MemoryBlock* new_block = (MemoryBlock*)((char*)block + sizeof(MemoryBlock) + size);
        new_block->size = block->size - size - sizeof(MemoryBlock);
        new_block->is_free = true;
        new_block->next = block->next;
        new_block->prev = block;
        if (block->next) {
            block->next->prev = new_block;
        }
        block->next = new_block;
        block->size = size;
    }
}

static void coalesce_blocks(MemoryBlock* block) {
    if (block->next && block->next->is_free) {
        block->size += sizeof(MemoryBlock) + block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
    if (block->prev && block->prev->is_free) {
        block->prev->size += sizeof(MemoryBlock) + block->size;
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
    }
}

void defragment_memory() {
    MemoryBlock* current = free_list;
    while (current && current->next) {
        if (current->is_free && current->next->is_free) {
            coalesce_blocks(current);
        } else {
            current = current->next;
        }
    }

    current = reserved_free_list;
    while (current && current->next) {
        if (current->is_free && current->next->is_free) {
            coalesce_blocks(current);
        } else {
            current = current->next;
        }
    }
}

void* kmalloc(size_t size, int flags) {
    size = align_size(size);
    MemoryBlock* current = (flags & KMALLOC_FLAG_RESERVED) ? reserved_free_list : free_list;

    // Debugging output
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Requesting %zu bytes from kmalloc\n", size);
    print_str(buffer);

    while (current) {
        if (current->is_free && current->size >= size) {
            split_block(current, size);
            current->is_free = false;

            // Remove from free list
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                if (flags & KMALLOC_FLAG_RESERVED) {
                    reserved_free_list = current->next;
                } else {
                    free_list = current->next;
                }
            }
            if (current->next) {
                current->next->prev = current->prev;
            }

            // Add to allocated list
            current->next = (flags & KMALLOC_FLAG_RESERVED) ? reserved_allocated_list : allocated_list;
            if (flags & KMALLOC_FLAG_RESERVED) {
                if (reserved_allocated_list) {
                    reserved_allocated_list->prev = current;
                }
                reserved_allocated_list = current;
            } else {
                if (allocated_list) {
                    allocated_list->prev = current;
                }
                allocated_list = current;
            }

            // Debugging output
            snprintf(buffer, sizeof(buffer), "Allocated %zu bytes at %p\n", size, (void*)((char*)current + sizeof(MemoryBlock)));
            print_str(buffer);

            return (void*)((char*)current + sizeof(MemoryBlock));
        }
        current = current->next;
    }

    last_error = ERR_NO_MEMORY;
    print_str("kmalloc failed: Out of memory\n");
    return NULL;
}

void kfree(void* ptr) {
    if (!ptr) return;
    MemoryBlock* block = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));
    block->is_free = true;

    // Remove from allocated list
    if (block->prev) {
        block->prev->next = block->next;
    } else {
        if (block >= (MemoryBlock*)memory_pool_start && block < (MemoryBlock*)(memory_pool_start + memory_pool_size / 4 * 3)) {
            allocated_list = block->next;
        } else {
            reserved_allocated_list = block->next;
        }
    }
    if (block->next) {
        block->next->prev = block->prev;
    }

    coalesce_blocks(block);
}

const char* get_error_message(int error_code) {
    switch (error_code) {
        case ERR_NO_MEMORY:
            return "Out of memory";
        default:
            return "Unknown error";
    }
}

void check_memory_leaks() {
    if (allocated_list || reserved_allocated_list) {
        print_str("Memory Leak Warning: The following blocks were not freed:\n");
        MemoryBlock* current = allocated_list;
        while (current) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Leaked Block: Size = %zu bytes\n", current->size);
            print_str(buffer);
            current = current->next;
        }
        current = reserved_allocated_list;
        while (current) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Leaked Block: Size = %zu bytes\n", current->size);
            print_str(buffer);
            current = current->next;
        }
    } else {
        print_str("No memory leaks detected.\n");
    }
}

void print_memory_stats() {
    size_t total_memory = memory_pool_size;
    size_t used_memory = 0;
    size_t free_memory = 0;
    size_t reserved_free_memory = 0;

    MemoryBlock* current = free_list;
    while (current) {
        if (current->is_free) {
            free_memory += current->size + sizeof(MemoryBlock);
        }
        current = current->next;
    }

    current = allocated_list;
    while (current) {
        if (!current->is_free) {
            used_memory += current->size + sizeof(MemoryBlock);
        }
        current = current->next;
    }

    current = reserved_free_list;
    while (current) {
        if (current->is_free) {
            reserved_free_memory += current->size + sizeof(MemoryBlock);
        }
        current = current->next;
    }

    current = reserved_allocated_list;
    while (current) {
        if (!current->is_free) {
            used_memory += current->size + sizeof(MemoryBlock);
        }
        current = current->next;
    }

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Total Memory: %zu bytes\n", total_memory);
    print_str(buffer);
    snprintf(buffer, sizeof(buffer), "Used Memory: %zu bytes\n", used_memory);
    print_str(buffer);
    snprintf(buffer, sizeof(buffer), "Free Memory: %zu bytes\n", free_memory);
    print_str(buffer);
    snprintf(buffer, sizeof(buffer), "Reserved Free Memory: %zu bytes\n", reserved_free_memory);
    print_str(buffer);
}

__attribute__((constructor))
static void setup_memory() {
    initialize_memory_manager();
}

__attribute__((destructor))
static void cleanup_memory() {
    check_memory_leaks();
}