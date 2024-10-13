global load_segment_selectors

section .text

load_segment_selectors:
    mov ax, 0x10       ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret