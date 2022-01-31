function1:
        mov     eax, esi
        cmp     edi, esi
        jle     .L2
        mov     eax, edi
.L2:
        ret
