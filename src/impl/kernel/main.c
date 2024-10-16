#include "vga.h"
#include "memory.h"
#include "keyboard.h"
#include "idt.h"

#define HEAP_SIZE 6 * 1024 * 1024 // 6 MiB

static char heap[HEAP_SIZE]; // Define the heap

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 64-bit kernel!\n");
    
    init_idt();
    
    // Initialize the memory manager
    memory_init(heap, HEAP_SIZE);

    init_keyboard();
}