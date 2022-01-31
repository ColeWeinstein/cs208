function1:
        test    edi, edi
        jle     .L3
        push    rbp
        push    rbx
        sub     rsp, 8
        mov     ebx, edi
        cmp     edi, 1
        je      .L4
        lea     edi, [rdi-1]
        call    function1
        mov     ebp, eax
        lea     edi, [rbx-2]
        call    function1
        add     eax, ebp
.L1:
        add     rsp, 8
        pop     rbx
        pop     rbp
        ret
.L3:
        mov     eax, 0
        ret
.L4:
        mov     eax, edi
        jmp     .L1