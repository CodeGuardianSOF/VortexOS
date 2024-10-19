#include "io.h"
#include "isr.h"
#include "vga.h"
#include "idt.h"
#include "memory.h"
#include "cli.h"
#include "delay.h"
#include "keyboard.h"
#include <stdint.h>
#include <stddef.h>

#define  CLI_PROMPT_LENGTH 5
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_IRQ 1
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

char scan_code_to_ascii(uint8_t scan_code, int shift, int caps_lock); // Function declaration

static int shift_pressed = 0; // Shift state
static int caps_lock_on = 0; // Caps Lock state
char *keyboard_buffer = NULL; // Define keyboard buffer

size_t buffer_head = 0; // Buffer head
size_t buffer_tail = 0; // Buffer tail

#define BUFFER_SIZE 1024 // Define the buffer size

extern char *buffer; // Declare buffer pointer
extern size_t buffer_head; // Declare buffer head
extern size_t buffer_tail; // Declare buffer tail

// Function to get the current cursor position
uint16_t get_cursor_position() {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

// Function to set the cursor position
void set_cursor_position(uint16_t pos) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Function to print a character at a specific position using VGA functions
void print_char_at(char c, uint16_t pos) {
    extern size_t col;
    extern size_t row;
    size_t old_col = col;
    size_t old_row = row;
    col = pos % SCREEN_WIDTH;
    row = pos / SCREEN_WIDTH;

    // Print the character using VGA functions
    print_char(c);

    // Restore the global cursor position
    col = old_col;
    row = old_row;
}

static void keyboard_callback(interrupt_frame *frame) {
    uint8_t scan_code = inb(KEYBOARD_DATA_PORT);

    color_reset(); // Reset text color

    // Handle special keys
    switch (scan_code) {
        case 0x1C: { // Enter key
            // Copy buffer content to command_buffer
            size_t i = 0;
            while (buffer_tail != buffer_head && i < MAX_COMMAND_LENGTH - 1) {
                command_buffer[i++] = keyboard_buffer[buffer_tail];
                buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
            }
            command_buffer[i] = '\0'; // Null-terminate the command

            // Pass the command to CLI for handling
            handle_command(command_buffer);

            // Clear the buffer and reset the cursor position
            buffer_head = buffer_tail = 0;
            break;
        }
        case 0x0E: { // Backspace key
            uint16_t pos = get_cursor_position();
            if (pos > CLI_PROMPT_LENGTH) { // Ensure cursor is not at the barrier
                if ((pos % SCREEN_WIDTH) != 0) { // Ensure cursor stays within the current row
                    set_cursor_position(pos - 1);
                    print_char_at(' ', pos - 1); // Overwrite the character with a space
                    set_cursor_position(pos - 1); // Move cursor back again
                } else { // Handle case where cursor is at the start of a line
                    // Barrier logic: Prevent moving back to the previous row
                    // Optionally, you can add a beep sound or some visual indication
                }
            }
            break;
        }
        case 0x2A: // Left Shift pressed
        case 0x36: // Right Shift pressed
            shift_pressed = 1;
            break;
        case 0xAA: // Left Shift released
        case 0xB6: // Right Shift released
            shift_pressed = 0;
            break;
        case 0x3A: // Caps Lock pressed
            caps_lock_on = !caps_lock_on;
            break;
        default:
            // Convert scan code to ASCII character (considering shift and caps lock)
            char ascii_char = scan_code_to_ascii(scan_code, shift_pressed, caps_lock_on);
            if (ascii_char != 0) {
                uint16_t pos = get_cursor_position();
                if (pos < SCREEN_WIDTH * SCREEN_HEIGHT) { // Ensure cursor does not exceed screen size
                    print_char_at(ascii_char, pos);
                    set_cursor_position(pos + 1); // Move cursor to the right

                    // Add character to buffer
                    keyboard_buffer[buffer_head] = ascii_char;
                    buffer_head = (buffer_head + 1) % BUFFER_SIZE;
                    // Handle buffer overflow
                    if (buffer_head == buffer_tail) {
                        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
                    }
                }
            }
            break;
    }

    // Send End of Interrupt (EOI) signal to PIC
    outb(0x20, 0x20);
}

void init_keyboard() {
    keyboard_buffer = (char *)kmalloc(BUFFER_SIZE, 0); // Allocate memory for the buffer
    buffer_head = buffer_tail = 0;

    // Set the cursor position to the barrier
    register_interrupt_handler(IRQ1, keyboard_callback);
}

void cleanup_keyboard() {
    if (keyboard_buffer) {
        kfree(keyboard_buffer); // Free the allocated buffer
        keyboard_buffer = NULL;
    }
}

char scan_code_to_ascii(uint8_t scan_code, int shift, int caps_lock) {
    static char ascii_table[] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
        '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    static char shift_ascii_table[] = {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
        '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
        '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (scan_code > 58) {
        return 0;
    }

    char ascii_char = shift ? shift_ascii_table[scan_code] : ascii_table[scan_code];

    if (caps_lock && ascii_char >= 'a' && ascii_char <= 'z') {
        ascii_char -= 32; // Convert to uppercase
    } else if (caps_lock && ascii_char >= 'A' && ascii_char <= 'Z') {
        ascii_char += 32; // Convert to lowercase
    }

    return ascii_char;
}

char read_char() {
    if (buffer_head == buffer_tail) {
        return 0; // Buffer is empty
    }
    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
    return c;
}