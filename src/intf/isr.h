#ifndef ISR_H
#define ISR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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

// External assembly stubs
extern void isr0_stub();
extern void isr1_stub();
extern void isr8_stub();
extern void isr13_stub();
extern void isr14_stub();
extern void isr32_stub();
extern void isr33_stub();
extern void isr34_stub();
extern void isr35_stub();
extern void isr36_stub();
extern void isr37_stub();
extern void isr38_stub();
extern void isr39_stub();
extern void isr40_stub();
extern void isr41_stub();
extern void isr42_stub();

// C interrupt handlers (called by the assembly stubs)
extern void isr0();
extern void isr1();
extern void isr8();
extern void isr13();
extern void isr14();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();

void enable_interrupts();
void handle_interrupt(int interrupt_number);
void reset_interrupt_flags();

#ifdef __cplusplus
}
#endif

#endif