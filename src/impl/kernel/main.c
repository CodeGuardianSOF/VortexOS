#include "idt.h"
#include "keyboard.h"
#include "vga.h"

void cause_divide_by_zero(); // Function declaration

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 64-bit kernel!\n");

    init_idt();
    init_keyboard();

    // Cause a divide by zero error
    // cause_divide_by_zero();
}

void cause_divide_by_zero() {
    int a = 1;
    int b = 0;
    int c = a / b; // This will cause a divide by zero error
}