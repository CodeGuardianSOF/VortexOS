#include "io.h"
#include "isr.h"
#include "vga.h"
#include "idt.h"
#include "memory.h"
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_IRQ 1
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define BUFFER_SIZE 1024 // Updated buffer size

char scan_code_to_ascii(uint8_t scan_code, int shift, int caps_lock); // Function declaration

static char* buffer = NULL; // Dynamically allocated buffer
static size_t buffer_head = 0;
static size_t buffer_tail = 0;
static int shift_pressed = 0; // Shift state
static int caps_lock_on = 0; // Caps Lock state

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

    // Handle special keys
    switch (scan_code) {
        case 0x1C: { // Enter key
            uint16_t pos = get_cursor_position();
            uint16_t new_pos = (pos / SCREEN_WIDTH + 1) * SCREEN_WIDTH;
            set_cursor_position(new_pos);
            break;
        }
        case 0x0E: { // Backspace key
            uint16_t pos = get_cursor_position();
            if (pos > 0) { // Ensure cursor is not at the start of the screen
                if ((pos % SCREEN_WIDTH) != 0) { // Ensure cursor stays within the current row
                    set_cursor_position(pos - 1);
                    print_char_at(' ', pos - 1); // Overwrite the character with a space
                    set_cursor_position(pos - 1); // Move cursor back again
                } else { // Handle case where cursor is at the start of a line
                    set_cursor_position(pos - 1);
                    print_char_at(' ', pos - 1); // Overwrite the character with a space
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
                    buffer[buffer_head] = ascii_char;
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
    buffer = (char*)kmalloc(BUFFER_SIZE); // Allocate memory for the buffer using malloc
    if (buffer == NULL) {
        // Handle memory allocation failure
        return;
    }
    register_interrupt_handler(IRQ1, keyboard_callback);
}

void cleanup_keyboard() {
    if (buffer != NULL) {
        kfree(buffer); // Free the allocated buffer using free
        buffer = NULL;
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

// Function to read a character from the buffer
char read_char() {
    if (buffer_head == buffer_tail) {
        return 0; // Buffer is empty
    }
    char c = buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
    return c;
}