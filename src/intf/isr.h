#ifndef ISR_H
#define ISR_H

#include <stdint.h>

struct interrupt_frame;

void isr0();  // Divide by Zero
void isr1();  // Debug
void isr8();  // Double Fault
void isr13(); // General Protection Fault
void isr14(); // Page Fault
void isr32(); // Timer
void isr33(); // Keyboard
void isr34(); // Cascade
void isr35(); // Custom ISR 35
void isr36(); // Custom ISR 36
void isr37(); // Custom ISR 37
void isr38(); // Custom ISR 38
void isr39(); // Custom ISR 39
void isr40(); // Custom ISR 40
void isr41(); // Custom ISR 41
void isr42(); // Custom ISR 42

void enable_interrupts();
void handle_interrupt(int interrupt_number);
void reset_interrupt_flags();

#endif