global CPUIDCHK:function
CPUIDCHK:
    push ebx
    pushfd
    pop eax
    mov ebx, eax
    xor eax, 0x200000
    push eax
    popfd
    pushfd
    pop eax
    xor eax, ebx
    je nocpuid
    mov eax, 1
    jmp restore
nocpuid:
    mov eax, 0
restore: 
    pop ebx
    ret
.endchk: 
