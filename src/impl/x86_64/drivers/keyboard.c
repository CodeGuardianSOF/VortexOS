#include "io.h"
#include "isr.h"
#include "vga.h"
#include "idt.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_IRQ 1

char scan_code_to_ascii(uint8_t scan_code); // Function declaration

static void keyboard_callback(interrupt_frame *frame) {
    uint8_t scan_code = inb(KEYBOARD_DATA_PORT);

    // Convert scan code to ASCII character (simplified, no shift/ctrl handling)
    char ascii_char = scan_code_to_ascii(scan_code);

    // Print the character to the screen
    if (ascii_char != 0) {
        print_char(ascii_char);
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
