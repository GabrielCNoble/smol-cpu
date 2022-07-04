mov stt, 0x100
start:
mov accw, 0x1
call test_func
jmp start
test_func:
add accw, 0x1
ret    


