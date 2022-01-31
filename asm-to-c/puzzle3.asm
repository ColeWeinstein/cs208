function1:
        test    rdi, rdi
        sete    al
        test    rsi, rsi
        sete    dl
        or      al, dl
        je      .L3
        ret
.L4:
        mov     BYTE PTR [rdi], al
        add     rdi, 1
        add     rsi, 1
.L3:
        movzx   eax, BYTE PTR [rsi]
        test    al, al
        jne     .L4
        mov     BYTE PTR [rdi], al
        ret
