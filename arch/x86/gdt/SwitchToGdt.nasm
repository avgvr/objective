BITS 32
section .text
global SWITCHTOGDT:function
SWITCHTOGDT:
        lgdt [esp + 4]
        jmp 0x08:.reloadCS
.reloadCS:
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        ret
.end:
