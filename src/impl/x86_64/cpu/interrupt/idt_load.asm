global idt_load
global enable_interrupts

section .text

idt_load:
    cli                   ; Disable interrupts
    lidt [rdi]            ; Load IDT using the address in rdi (64-bit mode uses rdi for first argument)
    ret                   ; Return

enable_interrupts:
    sti                   ; Enable interrupts
    ret                   ; Return