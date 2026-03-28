BITS 32
section .text
extern main, _estack, _init
global _start:function
_start:
            ; Stack setup
            mov eax, _estack
            ; Align stack to odd address
            and eax, 0xFFFFFFFE
            mov esp, eax

            call _init
            call main
            cli
.hang:      hlt
            jmp .hang
