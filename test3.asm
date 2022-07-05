mov stt, stack_start
loop:
mov accw, var
push accw
call test_func
pop accw
jmp loop

test_func:
    mov base, [stt + 0x2]
    mov accw, [base]
    sub accw, 1
    mov [base], accw
    ret


.byte(0x100)
stack_start:


var: .word 0xf