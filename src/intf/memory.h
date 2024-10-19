#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdbool.h>

void initialize_memory_manager();
void initialize_memory_pools(); // Add this forward declaration
void* kmalloc(size_t size, int flags); // Update this declaration
void kfree(void* ptr);
void print_memory_stats();
void compact_memory();

#endif // MEMORY_H