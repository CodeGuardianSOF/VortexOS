#include "vga.h"
#include "delay.h"
#include "memory.h" // Include the memory management header
#include "keyboard.h"
#include "idt.h"

#define HEAP_SIZE 1024 * 1024 // 1 MiB

static char heap[HEAP_SIZE]; // Define the heap

void cause_divide_by_zero(); // Function declaration

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 64-bit kernel!\n");
    
    init_idt();
    delay_sec(1);

    // Initialize the memory manager
    memory_init(heap, HEAP_SIZE);

    // Example usage of malloc and free
    void* ptr1 = malloc(100);
    if (ptr1) {
        print_str("Allocated 100 bytes of memory.\n");
    }

    void* ptr2 = malloc(200);
    if (ptr2) {
        print_str("Allocated 200 bytes of memory.\n");
    }

    free(ptr1);
    print_str("Freed 100 bytes of memory.\n");

    void* ptr3 = malloc(50);
    if (ptr3) {
        print_str("Allocated 50 bytes of memory.\n");
    }

    init_keyboard();
    delay_sec(1);
    color_reset();
}