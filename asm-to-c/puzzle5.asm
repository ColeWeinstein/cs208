function1:
        test    edi, edi
        jle     .L5
        mov     edx, 1
        mov     eax, 1
        mov     ecx, 0
        jmp     .L3
.L4:
        lea     esi, [rcx+rax]
        add     edx, 1
        mov     ecx, eax
        mov     eax, esi
.L3:
        cmp     edx, edi
        jl      .L4
        ret
.L5:
        mov     eax, 0
        ret
