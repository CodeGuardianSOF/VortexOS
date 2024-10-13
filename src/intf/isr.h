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
void isr33(struct interrupt_frame *frame); // Keyboard
void isr34(struct interrupt_frame *frame); // Cascade
void isr35(struct interrupt_frame *frame); // COM2
void isr36(struct interrupt_frame *frame); // COM1
void isr37(struct interrupt_frame *frame); // LPT2
void isr38(struct interrupt_frame *frame); // Floppy Disk
void isr39(struct interrupt_frame *frame); // LPT1
void isr40(struct interrupt_frame *frame); // CMOS
void isr41(struct interrupt_frame *frame); // Primary ATA
void isr42(struct interrupt_frame *frame); // Secondary ATA

void enable_interrupts();
void handle_interrupt(int interrupt_number);
void reset_interrupt_flags();

#endif