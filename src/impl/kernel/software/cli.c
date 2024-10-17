#include "keyboard.h"
#include "vga.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

void print_prompt() {
    const char *prompt = "cli> ";
    while (*prompt) {
        print_char(*prompt++);
    }
}

void handle_keyboard_input() {
    char c = read_char();
    if (c != 0) {
        print_char(c);
        if (c == '\n') {
            print_prompt();
        }
    }
}

void init_cli() {
    print_prompt();
}