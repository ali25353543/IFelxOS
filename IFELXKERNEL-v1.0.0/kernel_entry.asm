BITS 32

section .multiboot
align 8

header_start:
    dd 0xE85250D6
    dd 0
    dd header_end - header_start
    dd -(0xE85250D6 + 0 + (header_end - header_start))

; framebuffer request
align 8
    dw 5
    dw 0
    dd 20
    dd 1024
    dd 768
    dd 32

; end tag
align 8
    dw 0
    dw 0
    dd 8

header_end:

section .text
global start
extern kernel_main

start:
    mov esp, stack_top

    ; Pass Multiboot2 registers through to C kernel entry
    push ebx
    push eax
    call kernel_main
    add esp, 8

.hang:
    jmp .hang

section .bss
align 16

stack_bottom:
    resb 16384

stack_top:
