#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "asm.h"

struct reg_t regs[] = {
    {.name = "accl", .value = REG_ACCL},
    {.name = "acch", .value = REG_ACCH},
    {.name = "acc", .value = REG_ACCW},
    {.name = "accw", .value = REG_ACCW},
    {.name = "base", .value = REG_BASE},
    {.name = "stt", .value = REG_STT},
    {.name = "stb", .value = REG_STB},
};

uint16_t operand_width[] = {
    [REG_ACCL] = 1,
    [REG_ACCH] = 1,
    [REG_ACCH] = 2,
    [REG_BASE] = 2,
    [REG_STT] = 2,
    [REG_STB] = 2,
};

struct opcode_t opcodes[] = {
    [OPCODE_MOV]    = {
        .name = "mov",
        .variant_count = 26,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_MOV_ACCL_IMM8,  .width = 1, .operands[0] = REG_ACCL_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = ISA_OPCODE_MOV_ACCL_ACCH,  .width = 1, .operands[0] = REG_ACCL_BIT, .operands[1] = REG_ACCH_BIT},
            {.opcode = ISA_OPCODE_MOV_ACCH_IMM8,  .width = 1, .operands[0] = REG_ACCH_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = ISA_OPCODE_MOV_ACCH_ACCL,  .width = 1, .operands[0] = REG_ACCH_BIT, .operands[1] = REG_ACCL_BIT},
            {.opcode = ISA_OPCODE_MOV_ACCW_IMM16,  .width = 2, .operands[0] = REG_ACCW_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = ISA_OPCODE_MOV_ACCW_BASE,  .width = 2, .operands[0] = REG_ACCW_BIT, .operands[1] = REG_BASE_BIT},
            {.opcode = ISA_OPCODE_MOV_ACCW_STT,  .width = 2, .operands[0] = REG_ACCW_BIT, .operands[1] = REG_STT_BIT},
            {.opcode = ISA_OPCODE_MOV_ACC_STB,  .width = 2, .operands[0] = REG_ACCW_BIT, .operands[1] = REG_STB_BIT},
            {.opcode = ISA_OPCODE_MOV_BASE_IMM16,  .width = 2, .operands[0] = REG_BASE_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = ISA_OPCODE_MOV_BASE_ACCW,  .width = 2, .operands[0] = REG_BASE_BIT, .operands[1] = REG_ACCW_BIT},
            {.opcode = ISA_OPCODE_MOV_BASE_STT,  .width = 2, .operands[0] = REG_BASE_BIT, .operands[1] = REG_STT_BIT},
            {.opcode = ISA_OPCODE_MOV_BASE_STB,  .width = 2, .operands[0] = REG_BASE_BIT, .operands[1] = REG_STB_BIT},
            {.opcode = ISA_OPCODE_MOV_STT_IMM16,  .width = 2, .operands[0] = REG_STT_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = ISA_OPCODE_MOV_STT_ACCW,  .width = 2, .operands[0] = REG_STT_BIT, .operands[1] = REG_ACCW_BIT},
            {.opcode = ISA_OPCODE_MOV_STT_BASE,  .width = 2, .operands[0] = REG_STT_BIT, .operands[1] = REG_BASE_BIT},
            {.opcode = ISA_OPCODE_MOV_STT_STB,  .width = 2, .operands[0] = REG_STT_BIT, .operands[1] = REG_STB_BIT},
            {.opcode = ISA_OPCODE_MOV_STB_STT,  .width = 2, .operands[0] = REG_STB_BIT, .operands[1] = REG_STT_BIT},
            {.opcode = ISA_OPCODE_MOV_DST_I_IMM16,  .width = 2, .operands[0] = REG_ALL_BIT, .operands[1] = REG_CONST_BIT | REG_INDIRECT_BIT},
            {.opcode = ISA_OPCODE_MOV_DST_I_SRC,  .width = 2, .operands[0] = REG_ALL_BIT, .operands[1] = REG_ALL_BIT | REG_INDIRECT_BIT},
            {.opcode = ISA_OPCODE_MOV_DST_I_SRC_IMM16,  .width = 2, .operands[0] = REG_ALL_BIT, .operands[1] = REG_ALL_BIT | REG_CONST_BIT | REG_INDIRECT_BIT},

            {.opcode = ISA_OPCODE_MOV_I_IMM16_SRC16,  .width = 2, .operands[0] = REG_CONST_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL16_BIT},
            {.opcode = ISA_OPCODE_MOV_I_DST_SRC16,  .width = 2, .operands[0] = REG_ALL16_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL16_BIT},
            {.opcode = ISA_OPCODE_MOV_I_DST_IMM16_SRC16,  .width = 2, .operands[0] = REG_ALL16_BIT | REG_CONST_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL16_BIT},

            {.opcode = ISA_OPCODE_MOV_I_IMM16_SRC8,  .width = 1, .operands[0] = REG_CONST_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL8_BIT},
            {.opcode = ISA_OPCODE_MOV_I_DST_SRC8,  .width = 1, .operands[0] = REG_ALL16_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL8_BIT},
            {.opcode = ISA_OPCODE_MOV_I_DST_IMM16_SRC8,  .width = 1, .operands[0] = REG_ALL16_BIT | REG_CONST_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL8_BIT}
        }
    },
    [OPCODE_ADD]    = {
        .name = "add",
        .variant_count = 15,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_ADD_ACCL_IMM8, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_ADD_ACCL_ACCH, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_ACCH},

            {.opcode = ISA_OPCODE_ADD_ACCH_IMM8, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_ADD_ACCH_ACCL, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_ACCL},

            {.opcode = ISA_OPCODE_ADD_ACCW_IMM16, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_ADD_ACCW_BASE, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_BASE},
            {.opcode = ISA_OPCODE_ADD_ACCW_STT, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_STT},
            {.opcode = ISA_OPCODE_ADD_ACCW_STB, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_STB},

            {.opcode = ISA_OPCODE_ADD_BASE_IMM16, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_ADD_BASE_ACCW, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_ACCW},
            {.opcode = ISA_OPCODE_ADD_BASE_STT, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_STT},
            {.opcode = ISA_OPCODE_ADD_BASE_STB, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_STB},

            {.opcode = ISA_OPCODE_ADD_STT_IMM16, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_ADD_STT_ACCW, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_ACCW},
            {.opcode = ISA_OPCODE_ADD_STT_BASE, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_BASE},
        }
    },
    [OPCODE_SUB]    = {
        .name = "sub",
        .variant_count = 15,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_SUB_ACCL_IMM8, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_SUB_ACCL_ACCH, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_ACCH},

            {.opcode = ISA_OPCODE_SUB_ACCH_IMM8, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_SUB_ACCH_ACCL, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_ACCL},

            {.opcode = ISA_OPCODE_SUB_ACCW_IMM16, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_SUB_ACCW_BASE, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_BASE},
            {.opcode = ISA_OPCODE_SUB_ACCW_STT, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_STT},
            {.opcode = ISA_OPCODE_SUB_ACCW_STB, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_STB},

            {.opcode = ISA_OPCODE_SUB_BASE_IMM16, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_SUB_BASE_ACCW, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_ACCW},
            {.opcode = ISA_OPCODE_SUB_BASE_STT, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_STT},
            {.opcode = ISA_OPCODE_SUB_BASE_STB, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_STB},

            {.opcode = ISA_OPCODE_SUB_STT_IMM16, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_SUB_STT_ACCW, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_ACCW},
            {.opcode = ISA_OPCODE_SUB_STT_BASE, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_BASE},
        }
    },
    [OPCODE_MUL]    = {.name = "mul"},
    [OPCODE_DIV]    = {.name = "div"},
    [OPCODE_CMP]    = {
        .name = "cmp",
        .variant_count = 8,
        .variants = (struct opvariant_t[]) {
            {.opcode = ISA_OPCODE_CMP_ACCL_IMM8, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_CMP_ACCL_ACCH, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_ACCH},

            {.opcode = ISA_OPCODE_CMP_ACCH_IMM8, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_CMP_ACCH_ACCL, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_ACCH},

            {.opcode = ISA_OPCODE_CMP_ACCW_IMM16, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_CMP_ACCW_BASE, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_BASE},

            {.opcode = ISA_OPCODE_CMP_BASE_IMM16, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_CMP_BASE_ACCW, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_ACCW}
        }
    },
    [OPCODE_AND]    = {.name = "and"},
    [OPCODE_OR]     = {.name = "or"},
    
    [OPCODE_XOR]    = {.name = "xor"},
    [OPCODE_NOT]    = {.name = "not"},
    [OPCODE_SHL]    = {.name = "shl"},
    [OPCODE_SHR]    = {.name = "shr"},
    [OPCODE_INC]    = {.name = "inc"},
    [OPCODE_DEC]    = {.name = "dec"},
    [OPCODE_JMP]    = {
        .name = "jmp",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_JMP_IMM16, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },
    [OPCODE_JZ]     = {
        .name = "jz",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_JZ_JE_IMM16, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },
    [OPCODE_JE]     = {
        .name = "je",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_JZ_JE_IMM16, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },
    [OPCODE_JNZ]    = {
        .name = "jnz",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_JNZ_JNE_IMM16, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },
    [OPCODE_JNE]    = {
        .name = "jne",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_JNZ_JNE_IMM16, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },

    [OPCODE_JA]     = {.name = "ja"},
    [OPCODE_JNBE]   = {.name = "jnbe"},
    [OPCODE_JNA]    = {.name = "jna"},
    [OPCODE_JBE]    = {.name = "jbe"},
    [OPCODE_JAE]    = {.name = "jae"},
    [OPCODE_JNB]    = {.name = "jnb"},
    [OPCODE_JNC]    = {.name = "jnc"},
    [OPCODE_JNAE]   = {.name = "jnae"},
    [OPCODE_JB]     = {.name = "jb"},
    [OPCODE_JC]     = {.name = "jc"},  
    [OPCODE_JG]     = {.name = "jg"},
    [OPCODE_JL]     = {.name = "jl"},

    [OPCODE_CALL]   = {
        .name = "call",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_CALL_IMM16, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },
    [OPCODE_RET]    = {
        .name = "ret",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_RET, .width = 2}
        }
    },
    [OPCODE_PUSH]   = {
        .name = "push",
        .variant_count = 7,
        .variants = (struct opvariant_t []){
            // {.opcode = ISA_OPCODE_PUSH_IMM8, .width = 1, .operands[0] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_PUSH_IMM16, .width = 2, .operands[0] = 1 << REG_CONST},
            {.opcode = ISA_OPCODE_PUSH_ACCL, .width = 2, .operands[0] = 1 << REG_ACCL},
            {.opcode = ISA_OPCODE_PUSH_ACCH, .width = 2, .operands[0] = 1 << REG_ACCH},
            {.opcode = ISA_OPCODE_PUSH_ACCW, .width = 2, .operands[0] = 1 << REG_ACCW},
            {.opcode = ISA_OPCODE_PUSH_BASE, .width = 2, .operands[0] = 1 << REG_BASE},
            {.opcode = ISA_OPCODE_PUSH_STT, .width = 2, .operands[0] = 1 << REG_STT},
            {.opcode = ISA_OPCODE_PUSH_STB, .width = 2, .operands[0] = 1 << REG_STB},
        }
    },
    [OPCODE_POP]    = {
        .name = "pop",
        .variant_count = 6,
        .variants = (struct opvariant_t []){
            {.opcode = ISA_OPCODE_POP_ACCL, .width = 2, .operands[0] = 1 << REG_ACCL},
            {.opcode = ISA_OPCODE_POP_ACCH, .width = 2, .operands[0] = 1 << REG_ACCH},
            {.opcode = ISA_OPCODE_POP_ACCW, .width = 2, .operands[0] = 1 << REG_ACCW},
            {.opcode = ISA_OPCODE_POP_BASE, .width = 2, .operands[0] = 1 << REG_BASE},
            {.opcode = ISA_OPCODE_POP_STT, .width = 2, .operands[0] = 1 << REG_STT},
            {.opcode = ISA_OPCODE_POP_STB, .width = 2, .operands[0] = 1 << REG_STB},
        }
    },
    [OPCODE_IN]     = {
        .name = "in"
    },
    [OPCODE_OUT]    = {
        .name = "out",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = ISA_OPCODE_OUT_BASE_ACCL, .width = 1, .operands[0] = REG_BASE_BIT, .operands[1] = REG_ACCL_BIT}
        }
    }
};

struct opvariant_t variant_buffer[MAX_OPCODE_VARIANTS];

uint32_t punctuators[] = {
    [','] = PUNCTUATOR_COMMA,
    [':'] = PUNCTUATOR_COLLON,
    ['['] = PUNCTUATOR_LBRACE,
    [']'] = PUNCTUATOR_RBRACE,
    ['.'] = PUNCTUATOR_DOT,
    ['+'] = PUNCTUATOR_PLUS,
    ['-'] = PUNCTUATOR_MINUS,
    ['('] = PUNCTUATOR_LPARENTHESIS,
    [')'] = PUNCTUATOR_RPARENTHESIS
};

const char *keywords[] = {
    [KEYWORD_STRING]    = "string",
    [KEYWORD_BYTE]      = "byte",
    [KEYWORD_WORD]      = "word"
};

const char map[] = {
    ['a'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['b'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['c'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['d'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['e'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['f'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['g'] = CHAR_TYPE_KEYWORD,
    ['h'] = CHAR_TYPE_KEYWORD,
    ['i'] = CHAR_TYPE_KEYWORD,
    ['j'] = CHAR_TYPE_KEYWORD,
    ['k'] = CHAR_TYPE_KEYWORD,
    ['l'] = CHAR_TYPE_KEYWORD,
    ['m'] = CHAR_TYPE_KEYWORD,
    ['n'] = CHAR_TYPE_KEYWORD,
    ['o'] = CHAR_TYPE_KEYWORD,
    ['p'] = CHAR_TYPE_KEYWORD,
    ['q'] = CHAR_TYPE_KEYWORD,
    ['r'] = CHAR_TYPE_KEYWORD,
    ['s'] = CHAR_TYPE_KEYWORD,
    ['t'] = CHAR_TYPE_KEYWORD,
    ['u'] = CHAR_TYPE_KEYWORD,
    ['v'] = CHAR_TYPE_KEYWORD,
    ['w'] = CHAR_TYPE_KEYWORD,
    ['x'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['y'] = CHAR_TYPE_KEYWORD,
    ['z'] = CHAR_TYPE_KEYWORD,

    ['A'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['B'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['C'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['D'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['E'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['F'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['G'] = CHAR_TYPE_KEYWORD,
    ['H'] = CHAR_TYPE_KEYWORD,
    ['I'] = CHAR_TYPE_KEYWORD,
    ['J'] = CHAR_TYPE_KEYWORD,
    ['K'] = CHAR_TYPE_KEYWORD,
    ['L'] = CHAR_TYPE_KEYWORD,
    ['M'] = CHAR_TYPE_KEYWORD,
    ['N'] = CHAR_TYPE_KEYWORD,
    ['O'] = CHAR_TYPE_KEYWORD,
    ['P'] = CHAR_TYPE_KEYWORD,
    ['Q'] = CHAR_TYPE_KEYWORD,
    ['R'] = CHAR_TYPE_KEYWORD,
    ['S'] = CHAR_TYPE_KEYWORD,
    ['T'] = CHAR_TYPE_KEYWORD,
    ['Y'] = CHAR_TYPE_KEYWORD,
    ['V'] = CHAR_TYPE_KEYWORD,
    ['W'] = CHAR_TYPE_KEYWORD,
    ['X'] = CHAR_TYPE_CONSTANT | CHAR_TYPE_KEYWORD,
    ['Y'] = CHAR_TYPE_KEYWORD,
    ['Z'] = CHAR_TYPE_KEYWORD,

    ['_'] = CHAR_TYPE_KEYWORD,

    ['0'] = CHAR_TYPE_CONSTANT,
    ['1'] = CHAR_TYPE_CONSTANT,
    ['2'] = CHAR_TYPE_CONSTANT,
    ['3'] = CHAR_TYPE_CONSTANT,
    ['4'] = CHAR_TYPE_CONSTANT,
    ['5'] = CHAR_TYPE_CONSTANT,
    ['6'] = CHAR_TYPE_CONSTANT,
    ['7'] = CHAR_TYPE_CONSTANT,
    ['8'] = CHAR_TYPE_CONSTANT,
    ['9'] = CHAR_TYPE_CONSTANT,
    ['\\'] = CHAR_TYPE_LITERAL,
    ['\''] = CHAR_TYPE_CONSTANT | CHAR_TYPE_LITERAL,

    ['+'] = CHAR_TYPE_PUNCTUATOR,
    ['-'] = CHAR_TYPE_PUNCTUATOR,
    ['['] = CHAR_TYPE_PUNCTUATOR,
    [']'] = CHAR_TYPE_PUNCTUATOR,
    [','] = CHAR_TYPE_PUNCTUATOR,
    [':'] = CHAR_TYPE_PUNCTUATOR,
    ['('] = CHAR_TYPE_PUNCTUATOR,
    [')'] = CHAR_TYPE_PUNCTUATOR,
    ['.'] = CHAR_TYPE_PUNCTUATOR,
    ['\"'] = CHAR_TYPE_LITERAL,
};

uint32_t line_source_offset = 0;
uint32_t line = 1;
uint32_t column = 1;
uint32_t source_offset = 0;
uint32_t source_len = 0;
char *source = NULL;

struct token_t next_token = {
    .type = TOKEN_TYPE_SPACE
};

struct token_t cur_token = {
    .type = TOKEN_TYPE_SPACE
};

struct output_buffer_t *output_buffer = NULL;
struct output_buffer_t *cur_buffer = NULL;
uint32_t output_offset = 0;

struct label_t *labels = NULL;
struct label_t *last_label = NULL;

struct label_t *patches = NULL;
struct label_t *last_patch = NULL;

void lex_token()
{
    char token_text[64];
    uint32_t token_text_cursor = 0;

    cur_token = next_token;
    next_token.type = TOKEN_TYPE_SPACE;

    while(source_offset < source_len && map[source[source_offset]] == TOKEN_TYPE_SPACE)
    {
        if(source[source_offset] == '\n')
        {
            column = 0;
            line++;
            line_source_offset = source_offset;
        }

        column++;
        source_offset++;
    }

    if(map[source[source_offset]] == TOKEN_TYPE_SPACE)
    {
        next_token.type = TOKEN_TYPE_SPACE;
        return;
    }

    if(map[source[source_offset]] == CHAR_TYPE_PUNCTUATOR)
    {
        /* punctuator */
        next_token.type = TOKEN_TYPE_PUNCTUATOR;
        next_token.token = punctuators[source[source_offset]];
        next_token.line = line;
        next_token.column = column;

        source_offset++;
        column++;
    }
    else if(map[source[source_offset]] == CHAR_TYPE_LITERAL && source[source_offset] == '\"')
    {
        /* string literal */
        source_offset++;
        column++;

        next_token.type = TOKEN_TYPE_LITERAL;
        next_token.token = source_offset;
        next_token.line = line;
        next_token.column = column;

        while(source_offset < source_len && source[source_offset] != '\"')
        {
            if(source[source_offset] == '\n')
            {
                column = 0;
                line++;
                line_source_offset = source_offset;
            }

            column++;
            source_offset++;
        }
        if(source_offset >= source_len)
        {
            piss(PISS_ERROR, "Unexpected end of file reached while parsing string literal on line %d, column %d!", next_token.line, next_token.column);
        }
        next_token.length = source_offset - next_token.token;

        source_offset++;
        column++;
    }
    else
    {
        /* integer constant */
        uint32_t maybe_constant = 0;
        uint32_t constant_type = CONSTANT_NONE;
        uint32_t start_column = column;
        uint32_t start_line = line;
        uint32_t start_offset = source_offset;
        while(source_offset < source_len && token_text_cursor < sizeof(token_text) && 
                map[source[source_offset]] != CHAR_TYPE_SPACE && map[source[source_offset]] != CHAR_TYPE_PUNCTUATOR)
        {
            token_text[token_text_cursor] = source[source_offset];
            source_offset++;
            token_text_cursor++;
            column++;
        }

        token_text[token_text_cursor] = '\0';
        next_token.line = start_line;
        next_token.column = start_column;

        if(map[token_text[0]] & CHAR_TYPE_CONSTANT)
        {
            if(token_text_cursor > 2 && token_text[0] == '0' && (token_text[1] == 'b' || token_text[1] == 'B'))
            {
                /* binary constants starts with a 0b, and may be valid only if there's at least one character after
                the prefix */
                constant_type = CONSTANT_BINARY;
            }
            else if(token_text_cursor > 2 && token_text[0] == '0' && (token_text[1] == 'x' || token_text[1] == 'X'))
            {
                /* hex constants starts with a 0x, and may be valid only if there's at least one character after
                the prefix */
                constant_type = CONSTANT_HEXADECIMAL;
            }
            else if(token_text_cursor >= 3 && token_text_cursor <= 4 && token_text[0] == '\'' && token_text[token_text_cursor - 1] == '\'')
            {
                /* char constant is composed of a "'", followed either by a single character or 
                a '\' and a single character, followed by another "'" */

                if(token_text[1] == '\\')
                {
                    /* constant is escaping a char */
                    switch(token_text[2])
                    {
                        case '0':
                            token_text[1] = '\0';
                        break;

                        case 'n':
                            token_text[1] = '\n';
                        break;

                        case 't':
                            token_text[1] = '\t';
                        break;

                        default:
                            token_text[1] = token_text[2];
                        break;
                    }

                    token_text[2] = '\'';
                }
                constant_type = CONSTANT_CHAR;
            }
            else if(map[token_text[0]] == CHAR_TYPE_CONSTANT)
            {
                constant_type = CONSTANT_DECIMAL;
            }
            
            if(constant_type != CONSTANT_NONE && constant_type != CONSTANT_CHAR)
            {
                /* check if we don't have strange stuff inside a constant (only applicable
                if this isn't a char constant) */
                for(uint32_t index = 1; index < token_text_cursor; index++)
                {
                    if(!(map[token_text[index]] & CHAR_TYPE_CONSTANT))
                    {
                        piss(PISS_ERROR, "Uknown constant type one line %d, column %d!", start_line, start_column);
                    }
                }
            }
        }



        if(constant_type != CONSTANT_NONE)
        {
            uint32_t constant_value = 0;

            switch(constant_type)
            {
                case CONSTANT_DECIMAL:
                    constant_value = atoi(token_text);
                break;

                case CONSTANT_HEXADECIMAL:
                    constant_value = strtol(token_text, NULL, 16);
                break;

                case CONSTANT_CHAR:
                    /* we're just interested in the character in between "'" */
                    constant_value = token_text[1];
                break;

                case CONSTANT_BINARY:
                    /* 
                        get rid of the 0b prefix, since strtol won't accept it 
                        TODO: don't be lazy, drop the prefix when checking the constant type
                    */

                    token_text[0] = ' ';
                    token_text[1] = ' ';
                    constant_value = strtol(token_text, NULL, 2);
                break;
            }

            if(constant_value > 0xffff)
            {
                piss(PISS_WARNING, "Constant larger than 0xffff truncated to 0x%x, line %d, column %d!", (uint16_t)constant_value, line, column);
            }

            next_token.type = TOKEN_TYPE_CONSTANT;
            next_token.token = (uint16_t)constant_value;
        }
        else
        {
            
            /* test all opcodes */
            for(uint32_t index = 0; index < OPCODE_LAST; index++)
            {
                if(!strcmp(token_text, opcodes[index].name))
                {
                    next_token.type = TOKEN_TYPE_OPCODE;
                    next_token.token = index;
                    break;
                }
            }

            if(next_token.type == TOKEN_TYPE_SPACE)
            {
                /* test all regs */
                for(uint32_t index = 0; index < sizeof(regs) / sizeof(regs[0]); index++)
                {
                    if(!strcmp(token_text, regs[index].name))
                    {
                        next_token.type = TOKEN_TYPE_REG;
                        next_token.token = index;
                        break;
                    }
                }

                if(next_token.type == TOKEN_TYPE_SPACE)
                {

                    /* test all keywords */
                    for(uint32_t index = 0; index < KEYWORD_LAST; index++)
                    {
                        if(!strcmp(token_text, keywords[index]))
                        {
                            next_token.type = TOKEN_TYPE_KEYWORD,
                            next_token.token = index;
                            break;
                        }
                    }

                    if(next_token.type == TOKEN_TYPE_SPACE)
                    {
                        /* probably a label */
                        next_token.type = TOKEN_TYPE_NAME;
                        next_token.token = start_offset;
                        next_token.length = token_text_cursor;
                    }
                }
            }
        }
    }

    return;
}

void parse()
{
    lex_token();
    lex_token();

    while(source_offset < source_len)
    {
        if(cur_token.type == TOKEN_TYPE_OPCODE)
        {
            parse_instruction();
        }
        else if(cur_token.type == TOKEN_TYPE_NAME)
        {
            parse_label();
        }
        else if(cur_token.type == TOKEN_TYPE_PUNCTUATOR)
        {
            parse_declaration();
        }
    }
}

void filter_variants(struct opvariant_t *variant_buffer, uint32_t *variant_count, uint32_t operand, uint32_t mask, uint32_t result)
{
    uint32_t count = *variant_count;

    for(uint32_t index = 0; index < count; index++)
    {
        if((variant_buffer[index].operands[operand] & mask) != result)
        {
            variant_buffer[index] = variant_buffer[count - 1];
            count--;
            index--;
        }
    }

    *variant_count = count;
}

/*
    instruction:
        opcode
        opcode operand
        opcode operand, operand

    operand:
        term
        [term]
        [term + term]

    term:
        label-reference
        register
        constant

    label-reference:
        name
*/
void parse_instruction()
{
    uint32_t opcode_index = cur_token.token;
    struct opcode_t *opcode = opcodes + opcode_index;
    struct token_t constant_token = {.type = TOKEN_TYPE_SPACE};
    // uint32_t operands[2] = {0, 0};
    struct operand_t operands[2] = {};
    lex_token();

    memcpy(variant_buffer, opcode->variants, sizeof(struct opvariant_t) * opcode->variant_count);
    uint32_t variant_count = opcode->variant_count;

    // printf("%d %d\n", cur_token.type, cur_token.token);

    // if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token != PUNCTUATOR_LBRACE || cur_token.type != TOKEN_TYPE_PUNCTUATOR &&
    //    cur_token.type != TOKEN_TYPE_REG && cur_token.type != TOKEN_TYPE_CONSTANT && cur_token.type != TOKEN_TYPE_NAME)
    // {
    //     /* the current token isn't valid as part of an operand, so check if this instruction has a
    //     no operand variant */
    //     for(uint32_t index = 0; index < variant_count; index++)
    //     {
    //         if(variant_buffer[index].operands[0])
    //         {
    //             variant_buffer[index] = variant_buffer[variant_count - 1];
    //             variant_count--;
    //             index--;
    //         }
    //     }
        
    //     if(!variant_count)
    //     {
    //         piss(PISS_ERROR, "Missing first operand at line %d, column %d!", cur_token.line, cur_token.column);
    //     }
    // }
    // else
    {
        for(uint32_t operand_index = 0; operand_index < 2; operand_index++)
        {
            uint32_t term_count = 1;
            operands[operand_index].reg = REG_LAST;

            if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LBRACE)
            {
                uint32_t mask = 1 << REG_INDIRECT;
                operands[operand_index].indirect = 1;

                filter_variants(variant_buffer, &variant_count, operand_index, mask, mask);

                if(!variant_count)
                {
                    piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                }

                term_count++;
                lex_token();
            }
            else
            {
                uint32_t mask = 1 << REG_INDIRECT;
                filter_variants(variant_buffer, &variant_count, operand_index, mask, 0);

                if(!variant_count)
                {
                    piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                }
            }

            for(uint32_t term_index = 0; term_index < term_count; term_index++)
            {
                if(cur_token.type == TOKEN_TYPE_REG)
                {
                    /* register */

                    struct reg_t *reg = regs + cur_token.token;
                    if(operands[operand_index].reg != REG_LAST)
                    {
                        /* only one register per operand is allowed */
                        piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }

                    operands[operand_index].reg = reg->value;
                    lex_token();
                }
                else if(cur_token.type == TOKEN_TYPE_CONSTANT || (cur_token.type == TOKEN_TYPE_NAME && 
                       (next_token.type != TOKEN_TYPE_PUNCTUATOR || next_token.token != PUNCTUATOR_COLLON)))
                {
                    /* constant or a label reference */
                    uint32_t reg_index = 1 << REG_CONST;

                    if(operands[operand_index].constant)
                    {
                        /* only one constant per operand is allowed */
                        piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }

                    operands[operand_index].constant = 1;

                    constant_token = cur_token;
                    lex_token();
                }

                if(!term_index)
                {
                    if(operands[operand_index].indirect)
                    {
                        if(operands[operand_index].reg == REG_LAST && !operands[operand_index].constant)
                        {
                            /* there needs to be something inside the braces */
                            piss(PISS_ERROR, "Expecting register, constant or label at line %d, column %d!", cur_token.line, cur_token.column);
                        }

                        if(cur_token.type == TOKEN_TYPE_PUNCTUATOR)
                        {
                            if(cur_token.token == PUNCTUATOR_RBRACE)
                            {
                                /* this operand is composed by a single term, so we're done */
                                lex_token();
                                break;
                            }
                            else if(cur_token.token == PUNCTUATOR_PLUS)
                            {
                                /* we got a '+', so check for another term */
                                lex_token();

                                if(cur_token.type != TOKEN_TYPE_CONSTANT && cur_token.type != TOKEN_TYPE_REG && cur_token.type != TOKEN_TYPE_NAME)
                                {
                                    piss(PISS_ERROR, "Expecting register, constant or label after '+' at line %d, column %d!", cur_token.line, cur_token.column);    
                                }
                            }
                            else
                            {
                                piss(PISS_ERROR, "Expecting '+' or ']' at line %d, column %d!", cur_token.line, cur_token.column);
                            }
                        }
                        else
                        {
                            piss(PISS_ERROR, "Expecting '+' or ']' at line %d, column %d!", cur_token.line, cur_token.column);
                        }
                    }
                }
                else
                {
                    if(cur_token.type != TOKEN_TYPE_PUNCTUATOR || cur_token.token != PUNCTUATOR_RBRACE)
                    {
                        piss(PISS_ERROR, "Expecting ']' at line %d, column %d!", cur_token.line, cur_token.column);
                    }

                    lex_token();
                }
            }

            uint32_t mask;
            uint32_t result;

            if(operands[operand_index].reg == REG_LAST)
            {
                mask = REG_ALL_BIT;
                result = 0;
            }
            else
            {
                mask = 1 << operands[operand_index].reg;
                result = mask;
            }
            filter_variants(variant_buffer, &variant_count, operand_index, mask, result);

            mask = 1 << REG_CONST;
            result = 0;
            if(operands[operand_index].constant)
            {
                result = mask;    
            }

            filter_variants(variant_buffer, &variant_count, operand_index, mask, result);

            if(!operand_index)
            {
                if(!variant_count)
                {
                    if(!operands[0].constant && operands[0].reg == REG_LAST)
                    {
                        /* no variants, no first operand, no service! */
                        piss(PISS_ERROR, "Missing first operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }
                    else
                    {
                        piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }
                }

                if(!operands[0].constant && operands[0].reg == REG_LAST)
                {
                    /* we have no first operand but still have variants, which
                    means this instruction takes not operands */
                    break;
                }

                if(cur_token.type != TOKEN_TYPE_PUNCTUATOR || cur_token.token != PUNCTUATOR_COMMA)
                {
                    /* no comma after the first operand, so check if this instruction takes only one operand */
                    filter_variants(variant_buffer, &variant_count, 1, REG_ALL_BIT, 0);
                    
                    if(!variant_count)
                    {
                        piss(PISS_ERROR, "Missing second operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }

                    break;
                }
                else
                {
                    /* we found a ',' */
                    lex_token();
                }
            }
            else
            {
                if(!variant_count)
                {
                    if(!operands[1].constant && operands[1].reg == REG_LAST)
                    {
                        piss(PISS_ERROR, "Missing second operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }
                    else
                    {
                        piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }
                }
            }
        }
    }

    if((variant_buffer[0].opcode & WORD_OPCODE_MASK) == WORD_OPCODE_MASK)
    {
        variant_buffer[0].opcode |= (operands[1].reg | (operands[0].reg << 4)) << 8;
    }

    emit_opcode(variant_buffer[0].opcode);

    if(constant_token.type != TOKEN_TYPE_SPACE)
    {
        uint32_t constant = 0;

        if(constant_token.type == TOKEN_TYPE_NAME)
        {
            struct label_t *label = find_label_from_token(&constant_token);

            if(label)
            {
                constant = label->offset;
            }
            else
            {
                create_patch(&constant_token, output_offset);
            }
        }
        else
        {
            constant = constant_token.token;
        }

        if(variant_buffer[0].width == 1)
        {
            emit_byte(constant);
        }
        else
        {
            emit_word(constant);
        }
    }
}

/*
    label:
        name:
*/
void parse_label()
{
    struct token_t label_token = cur_token;
    lex_token();
    if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_COLLON)
    {
        /* this is a label */
        struct label_t *label = create_label(&label_token, output_offset);
        // printf("create label %s\n", label->name);
        lex_token();
    }
    else
    {
        piss(PISS_ERROR, "Garbage at line %d, column %d!", cur_token.line, cur_token.column);
    }
}

void parse_declaration()
{
    if(cur_token.token != PUNCTUATOR_DOT)
    {
        piss(PISS_ERROR, "Expecting '.' on line %d, column %d!", cur_token.line, cur_token.column);
    }

    lex_token();

    if(cur_token.type != TOKEN_TYPE_KEYWORD)
    {
        piss(PISS_ERROR, "Expecting keyword on line %d, column %d!", cur_token.line, cur_token.column);
    }

    struct token_t keyword = cur_token;

    lex_token();

    if(keyword.token == KEYWORD_STRING)
    {
        if(cur_token.type != TOKEN_TYPE_LITERAL)
        {
            piss(PISS_ERROR, "Expecting string literal at line %d, column %d!", cur_token.line, cur_token.column);
        }

        uint32_t offset = cur_token.token;
        for(uint32_t index = 0; index < cur_token.length; index++)
        {
            // char c = source[offset + index];

            if(source[offset + index] == '\n')
            {
                emit_byte(' ');
            }
            else if(index < cur_token.length - 1 && source[offset + index] == '\\')
            {
                if(source[offset + index + 1] == 'n')
                {
                    emit_byte('\n');
                }
                else if(source[offset + index + 1] == 't')
                {
                    emit_byte('\t');
                }
                else if(source[offset + index + 1] == '0')
                {
                    emit_byte('\0');
                }

                index++;
            }
            else
            {
                emit_byte(source[offset + index]);
            }
        }
        emit_byte('\0');
        lex_token();
        return;
    }
    else
    {
        uint32_t alloc_count = 1;

        if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LPARENTHESIS)
        {
            /* the keyword will receive a size "parameter" */
            lex_token();
            struct token_t constant = cur_token;

            if(constant.type != TOKEN_TYPE_CONSTANT)
            {
                printf(PISS_ERROR, "Expecting constant on line %d, column %d!", cur_token.line, cur_token.column);
            }

            lex_token();

            if(cur_token.type != TOKEN_TYPE_PUNCTUATOR || cur_token.token != PUNCTUATOR_RPARENTHESIS)
            {
                printf(PISS_ERROR, "Expecting ')' on line %d, column %d!", cur_token.line, cur_token.column);
            }

            alloc_count = constant.token;

            lex_token();
        }

        if(keyword.token == KEYWORD_BYTE)
        {
            uint32_t count = 0;
            while(cur_token.type == TOKEN_TYPE_CONSTANT)
            {
                count++;
                emit_byte(cur_token.token);
                lex_token();
            }

            while(count < alloc_count)
            {
                emit_byte(0);
                count++;
            }
        }
        else
        {
            uint32_t count = 0;
            while(cur_token.type == TOKEN_TYPE_CONSTANT)
            {
                count++;
                emit_word(cur_token.token);
                lex_token();
            }

            while(count < alloc_count)
            {
                emit_word(0);
                count++;
            }
        }
    }
}

void emit_byte(uint8_t byte)
{
    if(!output_buffer)
    {
        output_buffer = calloc(1, sizeof(struct output_buffer_t));
        output_buffer->data = calloc(1, OUTPUT_BUFFER_SIZE);
        cur_buffer = output_buffer;
    }

    cur_buffer->data[cur_buffer->offset] = byte;
    cur_buffer->offset++;

    if(cur_buffer->offset == OUTPUT_BUFFER_SIZE)
    {
        struct output_buffer_t *new_buffer = calloc(1, sizeof(struct output_buffer_t));
        new_buffer->data = calloc(1, OUTPUT_BUFFER_SIZE);
        cur_buffer->next = new_buffer;
        cur_buffer = new_buffer;
    }

    output_offset++;

    // printf("%x", byte);
}

void emit_word(uint16_t word)
{
    emit_byte((uint8_t)word);
    emit_byte((uint8_t)(word >> 8));
}

void emit_opcode(uint16_t opcode)
{
    if((opcode & WORD_OPCODE_MASK) == WORD_OPCODE_MASK)
    {
        emit_word(opcode);
    }
    else
    {
        emit_byte((uint8_t)opcode);
    }
}

void patch_code()
{
    struct output_buffer_t *buffer = output_buffer;
    uint32_t offset = OUTPUT_BUFFER_SIZE;

    struct label_t *patch = patches;

    while(patch)
    {
        struct label_t *label = find_label(patch->name);

        if(!label)
        {
            piss(PISS_ERROR, "Undefined label %s!", patch->name);
        }

        while(offset < patch->offset)
        {
            offset += buffer->offset;
            buffer = buffer->next;
        }

        uint32_t patch_offset = patch->offset % OUTPUT_BUFFER_SIZE;
        buffer->data[patch_offset] = (uint8_t)label->offset;
        buffer->data[patch_offset + 1] = (uint8_t)(label->offset >> 8);

        patch = patch->next;
    }
}

struct label_t *create_label_on_list(char *name, uint16_t name_len, uint16_t offset, struct label_t **first, struct label_t **last)
{
    struct label_t *label = calloc(1, sizeof(struct label_t ));

    if(name_len)
    {
        label->name = calloc(name_len + 1, 1);
        strncpy(label->name, name, name_len);
        label->name[name_len] = '\0';
    }
    else
    {
        label->name = strdup(name);
    }
    label->offset = offset;

    // printf("patch: %s\n", label->name);

    if(!(*first))
    {
        *first = label;
    }
    else
    {
        (*last)->next = label;
    }

    *last = label;
    return label;
}

struct label_t *create_label(struct token_t *token, uint16_t offset)
{
    struct label_t *label = create_label_on_list(source + token->token, token->length, offset, &labels, &last_label);
    return label;
}

struct label_t *create_patch(struct token_t *token, uint16_t offset)
{
    struct label_t *patch = create_label_on_list(source + token->token, token->length, offset, &patches, &last_patch);
    return patch;
}

struct label_t *find_label_from_token(struct token_t *token)
{
    struct label_t *label = labels;
    char *name = source + token->token;

    while(label)
    {
        if(!strncmp(label->name, name, token->length))
        {
            break;
        }

        label = label->next;
    }

    return label;
}

struct label_t *find_label(char *name)
{
    struct label_t *label = labels;

    while(label)
    {
        if(!strcmp(label->name, name))
        {
            break;
        }

        label = label->next;
    }

    return label;
}

void piss(uint32_t level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    switch(level)
    {
        case PISS_ERROR:
            printf("ERROR: ");
        break;

        case PISS_WARNING:
            printf("WARNING: ");
        break;

        case PISS_MESSAGE:
            printf("MESSAGE: ");
        break;
    }
    
    vprintf(fmt, args);
    printf("\n");
    va_end(args);

    if(level == PISS_ERROR)
    {
        exit(-1);
    }
}



int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        char *input = NULL;
        char *output = NULL;

        for(uint32_t arg_index = 1; arg_index < argc; arg_index++)
        {
            if(!strcmp(argv[arg_index], "-i"))
            {
                arg_index++;

                if(arg_index >= argc)
                {
                    piss(PISS_ERROR, "Missing input file name!");
                }

                input = argv[arg_index];
            }
            else if(!strcmp(argv[arg_index], "-o"))
            {
                arg_index++;

                if(arg_index >= argc)
                {
                    piss(PISS_ERROR, "Missing output file name!");
                }

                output = argv[arg_index];
            }
            else
            {
                piss(PISS_ERROR, "Unknown argument [%s]!", argv[arg_index]);
            }
        }


        if(!input)
        {
            piss(PISS_ERROR, "Missing input file name!");
        }

        if(!output)
        {
            piss(PISS_ERROR, "Missing output file name!");
        }

        FILE *file = fopen(input, "r");
        if(!file)
        {
            piss(PISS_ERROR, "Couldn't open file!");
        }

        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        rewind(file);

        source = calloc(1, file_size + 1); 
        fread((void *)source, file_size, 1, file);
        fclose(file);

        source_len = strlen(source) + 1;
        parse();
        patch_code();

        if(output)
        {
            FILE *output_file = fopen(output, "wb");
            struct output_buffer_t *buffer = output_buffer;
            uint32_t code_size = 0;
            while(buffer)
            {
                fwrite(buffer->data, 1, buffer->offset, output_file);
                code_size += buffer->offset;
                buffer = buffer->next;
            }
            fclose(output_file);
            piss(PISS_MESSAGE, "0x%x bytes generated", code_size);

            struct label_t *label = labels;
            while(label)
            {
                struct label_t *next_label = label->next;
                free(label->name);
                free(label);
                label = next_label;
            }

            struct label_t *patch = patches;
            while(patch)
            {
                struct label_t *next_patch = patch->next;
                free(patch->name);
                free(patch);
                patch = next_patch;
            }

            buffer = output_buffer;
            while(buffer)
            {
                struct output_buffer_t *next_buffer = buffer->next;
                free(buffer->data);
                free(buffer);
                buffer = next_buffer;
            }
        }
        else
        {
            piss(PISS_ERROR, "Input file empty!");
        }

        piss(PISS_MESSAGE, "Done!");

        return 0;
    }

    piss(PISS_ERROR, "No arguments provided!");
    return 0;
}