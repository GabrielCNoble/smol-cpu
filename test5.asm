mov stt, stack_start
loop:
push ze_string
call puts
pop accw
call clear
jmp loop

puts:
    mov base, [stt + 2]
    puts_loop:
        mov accl, [base]
        add base, 1
        cmp accl, 0
        jz done
        push base
        mov base, 0
        out base, accl
        pop base
        jmp puts_loop
    done:
    ret

clear:
    mov base, 0
    mov accl, 0x80
    out base, accl
    ret

.byte(0x100)
stack_start:

ze_string: .string "FLUTTER FLUTTER, MOFKR!!!!\n"
