start:
jmp end
middle:
mov accl, 0x1
mov acch, 0x2
mov accw, 0xffff
mov base, 0x1234
mov stt, 0x4321
mov stb, stt
mov stt, 0xabcd
jmp start
end:
mov accw, 0x00
mov base, 0x00
mov stt, 0x00
mov stb, stt
jmp middle




