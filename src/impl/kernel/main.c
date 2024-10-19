#include "memory.h"
#include "keyboard.h"
#include "idt.h"
#include "timer.h"
#include "vga.h"
#include "cli.h"
#include "scheduler.h"
#include "delay.h"
#include "manager.h" // Include the process manager header

void simple_process(); // Declare the simple process function

void simple_process() {
    while (1) {
        print_str("Simple process running!\n");
        // Add a delay to print every 20 seconds
        for (volatile int i = 0; i < 1200000000; i++);
    }
}


void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_LIGHT_CYAN, PRINT_COLOR_BLACK);
    print_str("Vortex starting!\n");

    color_reset();

    init_idt();

    initialize_memory_manager();

    initialize_process_manager();

    init_timer(100);
    
    create_process(init_keyboard, 1);

    create_process(init_cli, 0);

    create_process(print_memory_stats, 0);

    while (1) {
        schedule();
        task_scheduler();
    }
}

