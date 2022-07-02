mov base, 0
start:
mov accw, 0x0
loop:
cmp accw, 0x2
jz blah
add accw, 0x1
jmp loop
blah:
add base, 1
jmp start






