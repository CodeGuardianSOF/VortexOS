#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stddef.h>

void init_keyboard();
void cleanup_keyboard(); // Declare cleanup_keyboard
char scan_code_to_ascii(uint8_t scan_code, int shift, int caps_lock);
char read_char(); // Declare read_char

uint16_t get_cursor_position(); // Declare get_cursor_position
void set_cursor_position(uint16_t pos); // Declare set_cursor_position
void print_char_at(char c, uint16_t pos); // Declare print_char_at


#endif