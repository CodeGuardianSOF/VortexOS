#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void init_keyboard();
char scan_code_to_ascii(uint8_t scan_code);

#endif