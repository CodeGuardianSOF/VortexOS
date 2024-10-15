#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include "isr.h"

#define IDT_ENTRIES 256

#define IRQ0 32  // Timer
#define IRQ1 33  // Keyboard
// Add other IRQ definitions as needed

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_mid;
    uint32_t base_high;
    uint32_t reserved;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

extern void init_idt();
extern void (*interrupt_handlers[IDT_ENTRIES])(interrupt_frame *frame);
extern void register_interrupt_handler(uint8_t n, void (*handler)(interrupt_frame *frame));

#endif
