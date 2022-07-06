all: isa.exe asm.exe rom.bin

asm.exe: asm.c asm.h isa.h
	gcc asm.c -o asm.exe

isa.exe: isa.c isa.h
	gcc isa.c -o isa.exe

rom.bin: isa.h
	isa.exe
    