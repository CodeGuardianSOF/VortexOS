#include "isr.h"
#include "vga.h"
#include "delay.h"

extern void enable_interrupts_asm();

#define MAX_INTERRUPTS 256

static uint8_t interrupt_handled[MAX_INTERRUPTS] = {0};

void handle_interrupt(int interrupt_number) {
    if (interrupt_handled[interrupt_number] == 0) {
        print_set_color(PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
        print_str("Interrupt ");
        print_int(interrupt_number);
        print_str(" received!\n");
        interrupt_handled[interrupt_number] = 1;
    }
}

void isr0(struct interrupt_frame *frame) {
    handle_interrupt(0);
}

void isr1(struct interrupt_frame *frame) {
    handle_interrupt(1);
}

void isr8(struct interrupt_frame *frame) {
    handle_interrupt(8);
}

void isr13(struct interrupt_frame *frame) {
    handle_interrupt(13);
}

void isr14(struct interrupt_frame *frame) {
    handle_interrupt(14);
}

void isr32(struct interrupt_frame *frame) {
    handle_interrupt(32);
}