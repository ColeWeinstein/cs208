function1:
        mov     eax, edi
        test    edi, edi
        js      .L3
.L2:
        ret
.L3:
        neg     eax
        jmp     .L2
