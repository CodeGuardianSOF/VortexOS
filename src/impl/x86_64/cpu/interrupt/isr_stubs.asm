section .text
global isr0_stub, isr1_stub, isr8_stub, isr13_stub, isr14_stub, isr32_stub, isr33_stub, isr34_stub, isr35_stub, isr36_stub, isr37_stub, isr38_stub, isr39_stub, isr40_stub, isr41_stub, isr42_stub

extern isr0
extern isr1
extern isr8
extern isr13
extern isr14
extern isr32
extern isr33
extern isr34
extern isr35
extern isr36
extern isr37
extern isr38
extern isr39
extern isr40
extern isr41
extern isr42

%macro save_registers 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro restore_registers 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

isr0_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr0
    add rsp, 8
    restore_registers
    iretq

isr1_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr1
    add rsp, 8
    restore_registers
    iretq

isr8_stub:
    save_registers
    mov rdi, rsp
    cld
    call isr8
    restore_registers
    iretq

isr13_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr13
    add rsp, 8
    restore_registers
    iretq

isr14_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr14
    add rsp, 8
    restore_registers
    iretq

isr32_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr32
    add rsp, 8
    restore_registers
    iretq

isr33_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr33
    add rsp, 8
    restore_registers
    iretq

isr34_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr34
    add rsp, 8
    restore_registers
    iretq

isr35_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr35
    add rsp, 8
    restore_registers
    iretq

isr36_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr36
    add rsp, 8
    restore_registers
    iretq

isr37_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr37
    add rsp, 8
    restore_registers
    iretq

isr38_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr38
    add rsp, 8
    restore_registers
    iretq

isr39_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr39
    add rsp, 8
    restore_registers
    iretq

isr40_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr40
    add rsp, 8
    restore_registers
    iretq

isr41_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr41
    add rsp, 8
    restore_registers
    iretq

isr42_stub:
    save_registers
    push 0
    mov rdi, rsp
    cld
    call isr42
    add rsp, 8
    restore_registers
    iretq