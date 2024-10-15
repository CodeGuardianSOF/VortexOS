#ifndef ISR_H
#define ISR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t ds, es, fs, gs;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rds, rcs, rbx, rax;
    uint64_t int_no, error_code;
    uint64_t rip, cs, rflags, rsp, ss;
} interrupt_frame;

// External assembly stubs
extern void isr0_stub();
extern void isr1_stub();
extern void isr2_stub();
extern void isr3_stub();
extern void isr4_stub();
extern void isr5_stub();
extern void isr6_stub();
extern void isr7_stub();
extern void isr8_stub();
extern void isr9_stub();
extern void isr10_stub();
extern void isr11_stub();
extern void isr12_stub();
extern void isr13_stub();
extern void isr14_stub();
extern void isr15_stub();
extern void isr16_stub();
extern void isr17_stub();
extern void isr18_stub();
extern void isr19_stub();
extern void isr20_stub();
extern void isr21_stub();
extern void isr22_stub();
extern void isr23_stub();
extern void isr24_stub();
extern void isr25_stub();
extern void isr26_stub();
extern void isr27_stub();
extern void isr28_stub();
extern void isr29_stub();
extern void isr30_stub();
extern void isr31_stub();

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
extern void isr43_stub();
extern void isr44_stub();
extern void isr45_stub();
extern void isr46_stub();
extern void isr47_stub();

extern void enable_interrupts();
extern void handle_interrupt(interrupt_frame *frame);
extern void reset_interrupt_flags();

#ifdef __cplusplus
}
#endif

#endif
