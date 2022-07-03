zero_base:
mov base, 0
zero_acc:
mov accw, 0x0
loop:
add accw, 0x1
cmp accw, 0x5
jz inc_base
jmp loop
inc_base:
add base, 1
cmp base, 0x5
jz zero_base
jmp zero_acc






