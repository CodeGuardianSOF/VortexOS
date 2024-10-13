#ifndef ISR_H
#define ISR_H

#include <stdint.h>

struct interrupt_frame;

void isr0(struct interrupt_frame *frame);  // Divide by Zero
void isr1(struct interrupt_frame *frame);  // Debug
void isr8(struct interrupt_frame *frame);  // Double Fault
void isr13(struct interrupt_frame *frame); // General Protection Fault
void isr14(struct interrupt_frame *frame); // Page Fault
void isr32(struct interrupt_frame *frame); // Timer
void enable_interrupts();
void handle_interrupt(int interrupt_number);

#endif