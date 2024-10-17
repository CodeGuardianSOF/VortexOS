#ifndef CLI_H
#define CLI_H

#include <stdint.h>
#include <stdbool.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

void print_prompt();
void handle_keyboard_input();
void init_cli();

#endif // CLI_H