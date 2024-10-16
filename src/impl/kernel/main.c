#include "memory.h"
#include "keyboard.h"
#include "idt.h"
#include "timer.h"
#include "vga.h"
#include "delay.h"
#include "utils.h"
#include "scheduler.h" // Include the scheduler header

#define HEAP_SIZE 6 * 1024 * 1024 // 6 MiB

static char heap[HEAP_SIZE]; // Define the heap

void print_hello() {
    print_str("hello\n");
}

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 64-bit kernel!\n");
    
    init_idt();
    
    // Initialize the memory manager
    memory_init(heap, HEAP_SIZE);

    init_timer(100);

    init_keyboard();

    while (1) {
        scheduler_update();
    }
}