#include <stddef.h>
#include <stdint.h>
#include "memory.h"
#include "vga.h"

char *cmd_buffer = NULL; 
int cmd_n1 = 0;

void init_buffer(size_t buffer_size) {
    cmd_buffer = (char *)kmalloc(buffer_size, 0); // Allocate memory for cmd_buffer
    if (cmd_buffer != NULL) {
        cmd_buffer[0] = 'A'; // Insert a value into the buffer
        cmd_n1 = 1; // Update command length
    } else {
        cmd_n1 = 0; // Initialize command length
    }
}

void cleanup_buffer() {
    if (cmd_buffer != NULL) {
        kfree(cmd_buffer); // Free allocated memory
        cmd_buffer = NULL;
    }
}

void print_buffer() {
    if (cmd_buffer != NULL) {
        cmd_buffer[cmd_n1] = '\0'; // Null-terminate the buffer
        print_str(cmd_buffer); // Print the buffer content
    }
}