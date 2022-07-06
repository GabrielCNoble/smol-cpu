mov stt, stack_start
loop:
mov accw, var
push accw
call test_func
pop accw
jmp loop

test_func:
    mov base, [stt + 0x2]
    mov accl, [base]
    sub accl, 1
    mov [base], accl
    ret


.byte(0x100)
stack_start:


var: .word 0xf