#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void memory_init(void* heap_start, size_t heap_size);
void* malloc(size_t size);
void free(void* ptr);
void defragment_memory();

#endif // MEMORY_H