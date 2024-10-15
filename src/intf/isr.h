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

extern void enable_interrupts();
extern void handle_interrupt(interrupt_frame *frame);
extern void reset_interrupt_flags();

#ifdef __cplusplus
}
#endif

#endif
