#include "idt.h"
#include "keyboard.h"
#include "vga.h"
#include "delay.h"

void cause_divide_by_zero(); // Function declaration

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("Welcome to our 64-bit kernel!\n");

    init_idt();
    init_keyboard();
    delay_sec(1);
    color_reset();
}