section .text
extern main
extern _estack
global _start:function (_start.end - _start)
_start:
            mov esp, _estack
            call main
            cli
.hang:      hlt
            jmp .hang
.end:
