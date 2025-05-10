MBFLAGS equ 0
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

            section .multiboot
align 4
    dd MAGIC
    dd MBFLAGS
    dd CHECKSUM

section .bss
resb 512
align 16
; align 16 bytes according to the System V ABI standard
; Stack space
stack_bottom:
resb 16384 ; 16KiB
stack_top:

section .text
extern main
global _start:function (_start.end - _start)
_start:
            mov esp, stack_top
            call main
            cli
.hang:      hlt
            jmp .hang
.end:

section .kheap
kheap_top:
; Kernel heap space
; Change this value together Kheap::size varibale (mem/Kheap.h file)
resb 4096
kheap_bottom:
