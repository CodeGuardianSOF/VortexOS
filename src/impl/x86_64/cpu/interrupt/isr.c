#include "isr.h"
#include "vga.h"

#define MAX_INTERRUPTS 256

static uint8_t interrupt_handled[MAX_INTERRUPTS] = {0};

static const char* interrupt_messages[] = {
    "Divide by Zero",                // 0
    "Debug",                         // 1
    "Non-maskable Interrupt",        // 2
    "Breakpoint",                    // 3
    "Overflow",                      // 4
    "Bound Range Exceeded",          // 5
    "Invalid Opcode",                // 6
    "Device Not Available",          // 7
    "Double Fault",                  // 8
    "Coprocessor Segment Overrun",   // 9 (reserved)
    "Invalid TSS",                   // 10
    "Segment Not Present",           // 11
    "Stack-Segment Fault",           // 12
    "General Protection Fault",      // 13
    "Page Fault",                    // 14
    "Reserved",                      // 15
    "x87 Floating-Point Exception",  // 16
    "Alignment Check",               // 17
    "Machine Check",                 // 18
    "SIMD Floating-Point Exception", // 19
    "Virtualization Exception",      // 20
    "Reserved",                      // 21
    "Reserved",                      // 22
    "Reserved",                      // 23
    "Reserved",                      // 24
    "Reserved",                      // 25
    "Reserved",                      // 26
    "Reserved",                      // 27
    "Reserved",                      // 28
    "Reserved",                      // 29
    "Security Exception",            // 30
    "Reserved",                      // 31
    "Timer",                         // 32 (IRQ0)
    "Keyboard",                      // 33 (IRQ1)
    "Cascade",                       // 34 (IRQ2)
    "COM2",                          // 35 (IRQ3)
    "COM1",                          // 36 (IRQ4)
    "LPT2",                          // 37 (IRQ5)
    "Floppy Disk",                   // 38 (IRQ6)
    "LPT1",                          // 39 (IRQ7)
    "RTC",                           // 40 (IRQ8)
    "ACPI",                          // 41 (IRQ9)
    "PCI IRQ",                       // 42 (IRQ10)
};

void handle_interrupt(int interrupt_number) {
    if (interrupt_handled[interrupt_number] == 0) {
        print_set_color(PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
        print_str("Interrupt ");
        print_int(interrupt_number);
        print_str(" (");
        print_str(interrupt_messages[interrupt_number]);
        print_str(") received!\n");
        interrupt_handled[interrupt_number] = 1;
    }
}

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
void isr0() { handle_interrupt(0); }
void isr1() { handle_interrupt(1); }
void isr8() { handle_interrupt(8); }
void isr13() { handle_interrupt(13); }
void isr14() { handle_interrupt(14); }
void isr32() { handle_interrupt(32); }
void isr33() { handle_interrupt(33); }
void isr34() { handle_interrupt(34); }
void isr35() { handle_interrupt(35); }
void isr36() { handle_interrupt(36); }
void isr37() { handle_interrupt(37); }
void isr38() { handle_interrupt(38); }
void isr39() { handle_interrupt(39); }
void isr40() { handle_interrupt(40); }
void isr41() { handle_interrupt(41); }
void isr42() { handle_interrupt(42); }