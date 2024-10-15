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

isr0_stub:
    push 0               
    push rsp             
    call isr0           
    add rsp, 16          
    iretq                


isr1_stub:
    push 0
    push rsp
    call isr1
    add rsp, 16
    iretq


isr8_stub:
    push 0
    push rsp
    call isr8
    add rsp, 16
    iretq


isr13_stub:
    push 0
    push rsp
    call isr13
    add rsp, 16
    iretq


isr14_stub:
    push 0
    push rsp
    call isr14
    add rsp, 16
    iretq

isr32_stub:
    push 0
    push rsp
    call isr32
    add rsp, 16
    iretq

isr33_stub:
    push 0
    push rsp
    call isr33
    add rsp, 16
    iretq

isr34_stub:
    push 0
    push rsp
    call isr34
    add rsp, 16
    iretq

isr35_stub:
    push 0
    push rsp
    call isr35
    add rsp, 16
    iretq

isr36_stub:
    push 0
    push rsp
    call isr36
    add rsp, 16
    iretq

isr37_stub:
    push 0
    push rsp
    call isr37
    add rsp, 16
    iretq

isr38_stub:
    push 0
    push rsp
    call isr38
    add rsp, 16
    iretq

isr39_stub:
    push 0
    push rsp
    call isr39
    add rsp, 16
    iretq

isr40_stub:
    push 0
    push rsp
    call isr40
    add rsp, 16
    iretq

isr41_stub:
    push 0
    push rsp
    call isr41
    add rsp, 16
    iretq

isr42_stub:
    push 0
    push rsp
    call isr42
    add rsp, 16
    iretq
