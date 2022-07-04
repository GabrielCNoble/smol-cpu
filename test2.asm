mov stt, stack_start
start:
    mov accw, 0x1
    call test_func
    jmp start

test_func:
    add accw, 0x1
    cmp accw, 0xf
    jz done
    call test_func
    done:
    ret 


.word(0x100)
stack_start:

ze_string: .string "blah"


