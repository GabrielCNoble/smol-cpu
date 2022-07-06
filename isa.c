#include "isa.h"
#include <stdio.h>

int main()
{
    FILE *rom = fopen("rom.bin", "wb");
    fwrite(uop_rom, sizeof(uint16_t) * 16, ISA_OPCODE_LAST, rom); 
    fclose(rom);

    // for(uint32_t opcode = ISA_OPCODE_NOP; opcode < ISA_OPCODE_LAST; opcode++)
    // {
    //     // for(uint32_t uop = 0; uop < 16; uop++)
    //     // {
    //     //     // printf("%04x ", uop_rom[opcode][uop]);
            
    //     // }

    //     // printf("\n");
    // }
}