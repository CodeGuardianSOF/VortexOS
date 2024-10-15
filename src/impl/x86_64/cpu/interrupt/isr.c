#include "isr.h"
#include "idt.h"
#include "vga.h"
#include "io.h"

#define MAX_INTERRUPTS 256

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

void handle_interrupt(interrupt_frame *frame) {
    uint32_t interrupt_number = frame->int_no;

    /* Call the interrupt handler if one has been registered */
    if (interrupt_handlers[interrupt_number])
        interrupt_handlers[interrupt_number](frame);
    else {
        print_set_color(PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
        print_str("Unhandled interrupt ");
        print_int(interrupt_number);
        print_str(" (");
        print_str(interrupt_messages[interrupt_number]);
        print_str(") received!\n");
    }

    // Send End of Interrupt (EOI) signal to the PICs
    if (interrupt_number >= 32 && interrupt_number <= 47) {
        // Send reset signal to slave PIC if necessary
        if (interrupt_number >= 40) {
            outb(0xA0, 0x20); // 0xA0 is the command port for the slave PIC
        }
        // Send reset signal to master PIC
        outb(0x20, 0x20); // 0x20 is the command port for the master PIC
    }
}
