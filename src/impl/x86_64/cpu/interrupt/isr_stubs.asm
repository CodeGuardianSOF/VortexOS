section .text
extern handle_interrupt

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
    push gs
    push fs
    mov  rax, es
    push rax
    mov  rax, ds
    push rax
%endmacro

%macro restore_registers 0
    pop rax
    mov ds, rax
    pop rax
    mov es, rax
    pop fs
    pop gs
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

%macro isr_stub_error 1
global isr%1_stub
extern isr%1

isr%1_stub:
    push %1                    ; Push the ISR number on stack
    save_registers

    mov eax, 0x10              ; Load data selectors with kernel data selector
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    mov rdi, rsp
    cld
    call handle_interrupt

    restore_registers
    add rsp, 16                ; Remove interrupt numbeer and error
    iretq
%endmacro

%macro isr_stub_noerror 1
global isr%1_stub
extern isr%1

isr%1_stub:
    push 0                     ; Push dummy error code 0
    push %1                    ; Push the ISR number on stack
    save_registers

    mov eax, 0x10              ; Load data selectors with kernel data selector
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    mov rdi, rsp
    cld
    call handle_interrupt

    restore_registers
    add rsp, 16                ; Remove interrupt numbeer and error
    iretq
%endmacro

; Add exception handlers
isr_stub_noerror 0
isr_stub_noerror 1
isr_stub_error   8
isr_stub_error   13
isr_stub_error   14

; Add interrupt handlers
isr_stub_noerror 32
isr_stub_noerror 33
isr_stub_noerror 34
isr_stub_noerror 35
isr_stub_noerror 36
isr_stub_noerror 37
isr_stub_noerror 38
isr_stub_noerror 39
isr_stub_noerror 40
isr_stub_noerror 41
isr_stub_noerror 42
isr_stub_noerror 43
isr_stub_noerror 44
isr_stub_noerror 45
isr_stub_noerror 46
isr_stub_noerror 47
