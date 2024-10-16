#include "io.h"
#include "isr.h"
#include "vga.h"
#include "idt.h"
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_IRQ 1
#define SCREEN_WIDTH 80
#define BUFFER_SIZE 256

char scan_code_to_ascii(uint8_t scan_code); // Function declaration

static char buffer[BUFFER_SIZE];
static size_t buffer_head = 0;
static size_t buffer_tail = 0;

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

    // Convert scan code to ASCII character (simplified, no shift/ctrl handling)
    char ascii_char = scan_code_to_ascii(scan_code);

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
            if (pos > 0 && (pos % SCREEN_WIDTH) != 0) { // Ensure cursor stays within the current row
                set_cursor_position(pos - 1);
                print_char_at(' ', pos - 1); // Overwrite the character with a space
                set_cursor_position(pos - 1); // Move cursor back again
            }
            break;
        }
        default:
            // Print the character to the screen
            if (ascii_char != 0) {
                uint16_t pos = get_cursor_position();
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
            break;
    }

    // Send End of Interrupt (EOI) signal to PIC
    outb(0x20, 0x20);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

char scan_code_to_ascii(uint8_t scan_code) {
    static char ascii_table[] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
        '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (scan_code > 58) {
        return 0;
    }

    return ascii_table[scan_code];
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