#include "isr.h"
#include "vga.h"
#include "delay.h"

extern void enable_interrupts_asm();

#define MAX_INTERRUPTS 256

static uint8_t interrupt_handled[MAX_INTERRUPTS] = {0};

static const char* interrupt_messages[] = {
    "Divide by Zero",                // 0
    "Debug",                         // 1
    "Non Maskable Interrupt",        // 2
    "Breakpoint",                    // 3
    "Overflow",                      // 4
    "Bound Range Exceeded",          // 5
    "Invalid Opcode",                // 6
    "Device Not Available",          // 7
    "Double Fault",                  // 8
    "Coprocessor Segment Overrun",   // 9
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
    "Control Protection Exception",  // 21
    "Reserved",                      // 22
    "Reserved",                      // 23
    "Reserved",                      // 24
    "Reserved",                      // 25
    "Reserved",                      // 26
    "Reserved",                      // 27
    "Reserved",                      // 28
    "Reserved",                      // 29
    "Reserved",                      // 30
    "Reserved",                      // 31
    "Timer",                         // 32
    "Keyboard",                      // 33
    "Cascade",                       // 34
    "COM2",                          // 35
    "COM1",                          // 36
    "LPT2",                          // 37
    "Floppy Disk",                   // 38
    "LPT1",                          // 39
    "CMOS",                          // 40
    "Primary ATA",                   // 41
    "Secondary ATA"                  // 42
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

void isr0(struct interrupt_frame *frame) { handle_interrupt(0); }
void isr1(struct interrupt_frame *frame) { handle_interrupt(1); }
void isr8(struct interrupt_frame *frame) { handle_interrupt(8); }
void isr13(struct interrupt_frame *frame) { handle_interrupt(13); }
void isr14(struct interrupt_frame *frame) { handle_interrupt(14); }
void isr32(struct interrupt_frame *frame) { handle_interrupt(32); }
void isr33(struct interrupt_frame *frame) { handle_interrupt(33); }
void isr34(struct interrupt_frame *frame) { handle_interrupt(34); }
void isr35(struct interrupt_frame *frame) { handle_interrupt(35); }
void isr36(struct interrupt_frame *frame) { handle_interrupt(36); }
void isr37(struct interrupt_frame *frame) { handle_interrupt(37); }
void isr38(struct interrupt_frame *frame) { handle_interrupt(38); }
void isr39(struct interrupt_frame *frame) { handle_interrupt(39); }
void isr40(struct interrupt_frame *frame) { handle_interrupt(40); }
void isr41(struct interrupt_frame *frame) { handle_interrupt(41); }
void isr42(struct interrupt_frame *frame) { handle_interrupt(42); }