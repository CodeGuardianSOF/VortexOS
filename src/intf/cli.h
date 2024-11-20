#ifndef CLI_H
#define CLI_H

#include <stdint.h>
#include <stdbool.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_COMMAND_LENGTH 5

#ifndef NULL
#define NULL ((void*)0)
#endif

extern char *command_buffer; // Declare as extern
extern int command_length; // Declare as extern

void print_prompt();
void handle_keyboard_input();
void init_cli();
void cleanup_cli();
void handle_command(const char *command); // Add declaration for handle_command

#endif // CLI_H