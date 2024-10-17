#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

size_t get_available_memory_size();
void memory_init(void* heap_start, size_t heap_size);
void* kmalloc(size_t size);
void kfree(void* ptr);
void* realloc(void* ptr, size_t size);
void print_memory_stats();
void defragment_memory();

#endif // MEMORY_H