#include "keyboard.h"
#include "vga.h"
#include "cli.h"
#include "memory.h" // Include memory header for kmalloc and kfree

char *command_buffer = NULL; 
int command_length = 0; 
int cli_initialized = 0; // Flag to indicate if CLI is initialized
#define CLI_PROMPT_LENGTH 5

void print_prompt() {
    if (!cli_initialized) return; // Check if CLI is initialized
    print_set_color(PRINT_COLOR_LIGHT_CYAN, PRINT_COLOR_BLACK);
    uint16_t pos = get_cursor_position();
    // Ensure the cursor is at the start of the row
    pos = (pos / SCREEN_WIDTH) * SCREEN_WIDTH;
    const char *prompt = "cli> ";
    while (*prompt) {
        print_char_at(*prompt++, pos++);
    }
    set_cursor_position(pos); // Set cursor position after printing prompt
}

void print_str_at(const char *str, uint16_t pos) {
    if (!cli_initialized) return; // Check if CLI is initialized
    while (*str) {
        print_char_at(*str++, pos++);
        if (pos % SCREEN_WIDTH == 0) {
            pos = ((pos / SCREEN_WIDTH) + 1) * SCREEN_WIDTH; // Move to the start of the next row
        }
    }
    set_cursor_position(pos); // Set cursor position after printing the string
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void handle_command(const char *command) {
    if (!cli_initialized) return; // Check if CLI is initialized
    uint16_t start_pos = get_cursor_position();
    uint16_t pos = ((start_pos / SCREEN_WIDTH) + 1) * SCREEN_WIDTH; // Move to the start of the next row
    set_cursor_position(pos);

    if (strcmp(command, "help") == 0) {
        print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
        print_str_at("Available commands:\n", pos);
        pos = get_cursor_position();
        print_str_at("help - Show this help message\n", pos);
        print_str_at("memstats - Show memory statistics\n", pos);
    } else if (strcmp(command, "memstats") == 0) {
        print_memory_stats();
        // Move cursor 3 rows down after printing memstats
        pos = get_cursor_position();
        pos = ((pos / SCREEN_WIDTH) + 3) * SCREEN_WIDTH;
        set_cursor_position(pos);
    } else {
        print_set_color(PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
        print_str_at("Unknown command\n", pos);
    }
    pos = get_cursor_position();
    pos = ((pos / SCREEN_WIDTH) + 1) * SCREEN_WIDTH;
    set_cursor_position(pos);
    print_prompt(); // Print the prompt after moving to the next line
}

void handle_keyboard_input() {
    if (!cli_initialized) return; // Check if CLI is initialized
    char c = read_char();
    if (c != 0) {
        if (c == '\n') {
            command_buffer[command_length] = '\0'; // Null-terminate the command
            handle_command(command_buffer); // Handle the entered command
            command_length = 0; // Reset command length for the next input
        } else {
            if (command_length < MAX_COMMAND_LENGTH - 1) {
                command_buffer[command_length++] = c; // Store character in the buffer
                print_char(c); // Echo the character
            }
        }
    }
}

void init_cli() {
    command_buffer = (char *)kmalloc(MAX_COMMAND_LENGTH, 0); // Allocate memory for command_buffer
    cli_initialized = 1; // Set the CLI initialized flag to true
    
    set_cursor_position(CLI_PROMPT_LENGTH); // Set the cursor position to the barrier
    // print_clear(); // Clear the screen before showing the CLI
    // print_prompt(); // Print the prompt at the start
}

void cleanup_cli() {
    if (command_buffer != NULL) {
        kfree(command_buffer); // Free allocated memory
        command_buffer = NULL;
    }
    cli_initialized = 0; // Set the CLI initialized flag to false
}