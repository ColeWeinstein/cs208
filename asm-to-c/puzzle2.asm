function1:
        mov     eax, esi
        cmp     edi, esi
        jle     .L2
        mov     eax, edi
.L2:
        ret
function2:
        push    rbx
        mov     ebx, edx
        call    function1
        mov     edi, eax
        mov     esi, ebx
        call    function1
        pop     rbx
        ret
