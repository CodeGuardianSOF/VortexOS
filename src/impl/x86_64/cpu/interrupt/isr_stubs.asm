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

; ISR stub generation macro
; 1st Argument is the ISR number
; 2nd Argument optional error code to push
%macro isr_stub 1-2
global isr%1_stub
extern isr%1

isr%1_stub:
    ; Push a dummy error code if a 2nd parameter has been passed to the macro
%if %0 == 2
    push %2                    ; Push dummy error code
%endif
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

; Define stubs for exceptions (interrupts 0-31)
; Second parameter to isr_stub is the dummy error code (0) to push
;     for interrupts that the CPU doesn't push an error code for
isr_stub 0, 0                  ; Divide by Zero
isr_stub 1, 0                  ; Debug Exception
isr_stub 2, 0                  ; NMI Interrupt
isr_stub 3, 0                  ; Breakpoint
isr_stub 4, 0                  ; Overflow
isr_stub 5, 0                  ; Out of Bounds
isr_stub 6, 0                  ; Invalid Opcode
isr_stub 7, 0                  ; Device Not Available
isr_stub 8                     ; Double Fault
isr_stub 9, 0                  ; Coprocessor Segment Overrun
isr_stub 10                    ; Invalid TSS
isr_stub 11                    ; Segment Not Present
isr_stub 12                    ; Stack Fault
isr_stub 13                    ; General Protection Fault
isr_stub 14                    ; Page Fault
isr_stub 15, 0                 ; Reserved
isr_stub 16, 0                 ; x87 Floating Point Exception
isr_stub 17                    ; Alignment Check
isr_stub 18, 0                 ; Machine Check
isr_stub 19, 0                 ; SIMD Floating Point Exception
isr_stub 20, 0                 ; Reserved
isr_stub 21                    ; Reserved
isr_stub 22, 0                 ; Reserved
isr_stub 23, 0                 ; Reserved
isr_stub 24, 0                 ; Reserved
isr_stub 25, 0                 ; Reserved
isr_stub 26, 0                 ; Reserved
isr_stub 27, 0                 ; Reserved
isr_stub 28, 0                 ; Reserved
isr_stub 29                    ; Reserved
isr_stub 30                    ; Security Exception
isr_stub 31, 0                 ; Reserved

; Define stubs for interrupt handlers (IRQ0-IRQ15)
isr_stub 32, 0
isr_stub 33, 0
isr_stub 34, 0
isr_stub 35, 0
isr_stub 36, 0
isr_stub 37, 0
isr_stub 38, 0
isr_stub 39, 0
isr_stub 40, 0
isr_stub 41, 0
isr_stub 42, 0
isr_stub 43, 0
isr_stub 44, 0
isr_stub 45, 0
isr_stub 46, 0
isr_stub 47, 0
