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
        .variant_count = 23,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x01,    .width = 1, .operands[0] = REG_ACCL_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = 0x02,    .width = 1, .operands[0] = REG_ACCL_BIT, .operands[1] = REG_ACCH_BIT},
            {.opcode = 0x03,    .width = 2, .operands[0] = REG_ACCH_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = 0x04,    .width = 2, .operands[0] = REG_ACCH_BIT, .operands[1] = REG_ACCL_BIT},
            {.opcode = 0x05,    .width = 2, .operands[0] = REG_ACCW_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = 0x06,    .width = 2, .operands[0] = REG_ACCW_BIT, .operands[1] = REG_BASE_BIT},
            {.opcode = 0x07,    .width = 2, .operands[0] = REG_ACCW_BIT, .operands[1] = REG_STT_BIT},
            {.opcode = 0x08,    .width = 2, .operands[0] = REG_ACCW_BIT, .operands[1] = REG_STB_BIT},
            {.opcode = 0x09,    .width = 2, .operands[0] = REG_BASE_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = 0x0a,    .width = 2, .operands[0] = REG_BASE_BIT, .operands[1] = REG_ACCW_BIT},
            {.opcode = 0x0b,    .width = 2, .operands[0] = REG_BASE_BIT, .operands[1] = REG_STT_BIT},
            {.opcode = 0x0c,    .width = 2, .operands[0] = REG_BASE_BIT, .operands[1] = REG_STB_BIT},
            {.opcode = 0x0d,    .width = 2, .operands[0] = REG_STT_BIT, .operands[1] = REG_CONST_BIT},
            {.opcode = 0x0e,    .width = 2, .operands[0] = REG_STT_BIT, .operands[1] = REG_ACCW_BIT},
            {.opcode = 0x0f,    .width = 2, .operands[0] = REG_STT_BIT, .operands[1] = REG_BASE_BIT},
            {.opcode = 0x10,    .width = 2, .operands[0] = REG_STT_BIT, .operands[1] = REG_STB_BIT},
            {.opcode = 0x11,    .width = 2, .operands[0] = REG_STB_BIT, .operands[1] = REG_STT_BIT},
            {.opcode = 0x00c0,  .width = 2, .operands[0] = REG_ALL_BIT, .operands[1] = REG_CONST_BIT | REG_INDIRECT_BIT},
            {.opcode = 0x00c1,  .width = 2, .operands[0] = REG_ALL_BIT, .operands[1] = REG_ALL_BIT | REG_INDIRECT_BIT},
            {.opcode = 0x00c2,  .width = 2, .operands[0] = REG_ALL_BIT, .operands[1] = REG_ALL_BIT | REG_CONST_BIT | REG_INDIRECT_BIT},

            {.opcode = 0x00c3,  .width = 2, .operands[0] = REG_CONST_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL_BIT},
            {.opcode = 0x00c4,  .width = 2, .operands[0] = REG_ALL_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL_BIT},
            {.opcode = 0x00c5,  .width = 2, .operands[0] = REG_ALL_BIT | REG_CONST_BIT | REG_INDIRECT_BIT, .operands[1] = REG_ALL_BIT}
        }
    },
    [OPCODE_ADD]    = {
        .name = "add",
        .variant_count = 15,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x12, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x13, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_ACCH},

            {.opcode = 0x14, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x15, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_ACCL},

            {.opcode = 0x16, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x17, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_BASE},
            {.opcode = 0x18, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_STT},
            {.opcode = 0x19, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_STB},

            {.opcode = 0x1a, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x1b, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_ACCW},
            {.opcode = 0x1c, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_STT},
            {.opcode = 0x1d, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_STB},

            {.opcode = 0x1e, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x1f, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_ACCW},
            {.opcode = 0x20, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_BASE},
        }
    },
    [OPCODE_SUB]    = {
        .name = "sub",
        .variant_count = 15,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x21, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x22, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_ACCH},

            {.opcode = 0x23, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x24, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_ACCL},

            {.opcode = 0x25, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x26, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_BASE},
            {.opcode = 0x27, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_STT},
            {.opcode = 0x28, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_STB},

            {.opcode = 0x29, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x2a, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_ACCW},
            {.opcode = 0x2b, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_STT},
            {.opcode = 0x2c, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_STB},

            {.opcode = 0x2d, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x2e, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_ACCW},
            {.opcode = 0x2f, .width = 2, .operands[0] = 1 << REG_STT, .operands[1] = 1 << REG_BASE},
        }
    },
    [OPCODE_MUL]    = {.name = "mul"},
    [OPCODE_DIV]    = {.name = "div"},
    [OPCODE_CMP]    = {
        .name = "cmp",
        .variant_count = 8,
        .variants = (struct opvariant_t[]) {
            {.opcode = 0x3b, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x3c, .width = 1, .operands[0] = 1 << REG_ACCL, .operands[1] = 1 << REG_ACCH},

            {.opcode = 0x3d, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x3e, .width = 1, .operands[0] = 1 << REG_ACCH, .operands[1] = 1 << REG_ACCH},

            {.opcode = 0x3f, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x40, .width = 2, .operands[0] = 1 << REG_ACCW, .operands[1] = 1 << REG_BASE},

            {.opcode = 0x41, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_CONST},
            {.opcode = 0x42, .width = 2, .operands[0] = 1 << REG_BASE, .operands[1] = 1 << REG_ACCW}
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
            {.opcode = 0x77, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },
    [OPCODE_JZ]     = {
        .name = "jz",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x78, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },
    [OPCODE_JNZ]    = {.name = "jnz"},
    [OPCODE_CALL]   = {
        .name = "call",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x87, .width = 2, .operands[0] = 1 << REG_CONST}
        }
    },
    [OPCODE_RET]    = {
        .name = "ret",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x88, .width = 2}
        }
    },
    [OPCODE_PUSH]   = {
        .name = "push",
        .variant_count = 8,
        .variants = (struct opvariant_t []){
            // {.opcode = 0x89, .width = 1, .dst_reg = 0xffff},
            {.opcode = 0x8a, .width = 2, .operands[0] = 1 << REG_CONST},
            {.opcode = 0x8b, .width = 2, .operands[0] = 1 << REG_ACCL},
            {.opcode = 0x8c, .width = 2, .operands[0] = 1 << REG_ACCH},
            {.opcode = 0x8d, .width = 2, .operands[0] = 1 << REG_ACCW},
            {.opcode = 0x8e, .width = 2, .operands[0] = 1 << REG_BASE},
            {.opcode = 0x8f, .width = 2, .operands[0] = 1 << REG_STT},
            {.opcode = 0x90, .width = 2, .operands[0] = 1 << REG_STB},
        }
    },
    [OPCODE_POP]    = {
        .name = "pop",
        .variant_count = 6,
        .variants = (struct opvariant_t []){
            {.opcode = 0x91, .width = 2, .operands[0] = 1 << REG_ACCL},
            {.opcode = 0x92, .width = 2, .operands[0] = 1 << REG_ACCH},
            {.opcode = 0x93, .width = 2, .operands[0] = 1 << REG_ACCW},
            {.opcode = 0x94, .width = 2, .operands[0] = 1 << REG_BASE},
            {.opcode = 0x95, .width = 2, .operands[0] = 1 << REG_STT},
            {.opcode = 0x96, .width = 2, .operands[0] = 1 << REG_STB},
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

void next_token()
{
    char token_text[64];
    uint32_t token_text_cursor = 0;

    cur_token.type = TOKEN_TYPE_SPACE;
    // cur_token.last_in_line = 0;

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
        cur_token.type = TOKEN_TYPE_SPACE;
        return;
    }

    if(map[source[source_offset]] == CHAR_TYPE_PUNCTUATOR)
    {
        /* punctuator */
        cur_token.type = TOKEN_TYPE_PUNCTUATOR;
        cur_token.token = punctuators[source[source_offset]];
        cur_token.line = line;
        cur_token.column = column;

        source_offset++;
        column++;
    }
    else if(map[source[source_offset]] == CHAR_TYPE_LITERAL && source[source_offset] == '\"')
    {
        /* string literal */
        source_offset++;
        column++;

        cur_token.type = TOKEN_TYPE_LITERAL;
        cur_token.token = source_offset;
        cur_token.line = line;
        cur_token.column = column;

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
            piss(PISS_ERROR, "Unexpected end of file reached while parsing string literal on line %d, column %d!", cur_token.line, cur_token.column);
        }
        cur_token.length = source_offset - cur_token.token;

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
        cur_token.line = start_line;
        cur_token.column = start_column;

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

            cur_token.type = TOKEN_TYPE_CONSTANT;
            cur_token.token = (uint16_t)constant_value;
        }
        else
        {
            
            /* test all opcodes */
            for(uint32_t index = 0; index < OPCODE_LAST; index++)
            {
                if(!strcmp(token_text, opcodes[index].name))
                {
                    cur_token.type = TOKEN_TYPE_OPCODE;
                    cur_token.token = index;
                    break;
                }
            }

            if(cur_token.type == TOKEN_TYPE_SPACE)
            {
                /* test all regs */
                for(uint32_t index = 0; index < sizeof(regs) / sizeof(regs[0]); index++)
                {
                    if(!strcmp(token_text, regs[index].name))
                    {
                        cur_token.type = TOKEN_TYPE_REG;
                        cur_token.token = index;
                        break;
                    }
                }

                if(cur_token.type == TOKEN_TYPE_SPACE)
                {

                    /* test all keywords */
                    for(uint32_t index = 0; index < KEYWORD_LAST; index++)
                    {
                        if(!strcmp(token_text, keywords[index]))
                        {
                            cur_token.type = TOKEN_TYPE_KEYWORD,
                            cur_token.token = index;
                            break;
                        }
                    }

                    if(cur_token.type == TOKEN_TYPE_SPACE)
                    {
                        /* probably a label */
                        cur_token.type = TOKEN_TYPE_NAME;
                        cur_token.token = start_offset;
                        cur_token.length = token_text_cursor;
                    }
                }
            }
        }
    }

    /* check if this token is the last in line, to give the parser some additional information */
    // while(source_offset < source_len && map[source[source_offset]] == TOKEN_TYPE_SPACE)
    // {
    //     if(source[source_offset] == '\n')
    //     {
    //         cur_token.last_in_line = 1;
    //         break;
    //     }

    //     column++;
    //     source_offset++;
    // }

    return;
}

void parse()
{
    next_token();

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

void parse_instruction()
{
    uint32_t opcode_index = cur_token.token;
    struct opcode_t *opcode = opcodes + opcode_index;
    struct token_t constant_token = {.type = TOKEN_TYPE_SPACE};
    uint32_t operands[2] = {0, 0};
    next_token();

    memcpy(variant_buffer, opcode->variants, sizeof(struct opvariant_t) * opcode->variant_count);
    uint32_t variant_count = opcode->variant_count;

    // printf("%d %d\n", cur_token.type, cur_token.token);

    if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token != PUNCTUATOR_LBRACE || cur_token.type != TOKEN_TYPE_PUNCTUATOR &&
       cur_token.type != TOKEN_TYPE_REG && cur_token.type != TOKEN_TYPE_CONSTANT && cur_token.type != TOKEN_TYPE_NAME)
    {
        /* the current token isn't valid as part of an operand, so check if this instruction has a
        no operand variant */
        for(uint32_t index = 0; index < variant_count; index++)
        {
            if(variant_buffer[index].operands[0])
            {
                variant_buffer[index] = variant_buffer[variant_count - 1];
                variant_count--;
                index--;
            }
        }
        
        if(!variant_count)
        {
            piss(PISS_ERROR, "Missing first operand at line %d, column %d!", cur_token.line, cur_token.column);
        }
    }
    else
    {
        for(uint32_t operand_index = 0; operand_index < 2; operand_index++)
        {
            uint32_t mem_operand = 0;
            uint32_t term_count = 1;

            if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LBRACE)
            {
                uint32_t reg_index = 1 << REG_INDIRECT;
                operands[operand_index] |= reg_index;
                /* filter out all opcodes where the current operand isn't memory */ 
                for(uint32_t index = 0; index < variant_count; index++)
                {
                    if(!(variant_buffer[index].operands[operand_index] & operands[operand_index]))
                    {
                        variant_buffer[index] = variant_buffer[variant_count - 1];
                        variant_count--;
                        index--;
                    }
                }

                if(!variant_count)
                {
                    piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                }

                mem_operand = 1;
                term_count++;
                next_token();
            }
            else
            {
                uint32_t reg_index = 1 << REG_INDIRECT;
                /* filter out all opcodes where the current operand is memory */
                for(uint32_t index = 0; index < variant_count; index++)
                {
                    if(variant_buffer[index].operands[operand_index] & reg_index)
                    {
                        variant_buffer[index] = variant_buffer[variant_count - 1];
                        variant_count--;
                        index--;
                    }
                }
            }

            /* if this is a memory operand, there can be a constant, a register or a 
            register + a constant inside braces. Otherwise, it may be a register or a constant */
            for(uint32_t term_index = 0; term_index < term_count; term_index++)
            {
                if(cur_token.type == TOKEN_TYPE_REG)
                {
                    /* register */

                    struct reg_t *reg = regs + cur_token.token;
                    if(operands[operand_index] & reg->value)
                    {
                        piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }

                    /* we store the index of the register, instead of its bit, because we need 
                    to recover it later when emitting the opcode */
                    operands[operand_index] |= reg->value;
                    uint32_t reg_index = 1 << reg->value;

                    for(uint32_t index = 0; index < variant_count; index++)
                    {
                        if(!(variant_buffer[index].operands[operand_index] & reg_index))
                        {
                            variant_buffer[index] = variant_buffer[variant_count - 1];
                            variant_count--;
                            index--;
                        }
                    }

                    if(!variant_count)
                    {
                        piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }

                    next_token();
                }
                else if(cur_token.type == TOKEN_TYPE_CONSTANT || cur_token.type == TOKEN_TYPE_NAME)
                {
                    /* constant or a label */
                    uint32_t reg_index = 1 << REG_CONST;

                    if(operands[operand_index] & reg_index)
                    {
                        piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }
                    operands[operand_index] |= reg_index;

                    for(uint32_t index = 0; index < variant_count; index++)
                    {
                        if(!(variant_buffer[index].operands[operand_index] & reg_index))
                        {
                            variant_buffer[index] = variant_buffer[variant_count - 1];
                            variant_count--;
                            index--;
                        }
                    }

                    if(!variant_count)
                    {
                        piss(PISS_ERROR, "Invalid operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }
                    constant_token = cur_token;
                    next_token();
                }

                if(!term_index)
                {
                    if(!(operands[operand_index] & ~(REG_INDIRECT_BIT)))
                    {
                        piss(PISS_ERROR, "Expecting register, constant or label at line %d, column %d!", cur_token.line, cur_token.column);
                    }

                    if(mem_operand)
                    {
                        if(cur_token.type == TOKEN_TYPE_PUNCTUATOR)
                        {
                            if(cur_token.token == PUNCTUATOR_RBRACE)
                            {
                                /* we're done here */
                                next_token();
                                break;
                            }
                            else if(cur_token.token == PUNCTUATOR_PLUS)
                            {
                                /* we got a '+' */
                                next_token();

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

                    next_token();
                }

                // if(mem_operand)
                // {
                //     if(!term_index)
                //     {
                //         if(!(operands[operand_index] & ~(REG_INDIRECT_BIT)))
                //         {
                //             piss(PISS_ERROR, "Expecting operand at line %d, column %d!", cur_token.line, cur_token.column);
                //         }
                //         else if(cur_token.type == TOKEN_TYPE_PUNCTUATOR)
                //         {
                //             if(cur_token.token == PUNCTUATOR_RBRACE)
                //             {
                //                 /* we're done here */
                //                 next_token();
                //                 break;
                //             }
                //             else if(cur_token.token == PUNCTUATOR_PLUS)
                //             {
                //                 /* we'll be adding something to what we have so far */
                //                 next_token();

                //                 if(cur_token.type != TOKEN_TYPE_CONSTANT && cur_token.type != TOKEN_TYPE_REG && cur_token.type != TOKEN_TYPE_NAME)
                //                 {
                //                     piss(PISS_ERROR, "Expecting register, constant or label after '+' at line %d, column %d!", cur_token.line, cur_token.column);    
                //                 }
                //             }
                //             else
                //             {
                //                 piss(PISS_ERROR, "Expecting '+' or ']' at line %d, column %d!", cur_token.line, cur_token.column);
                //             }    
                //         }    
                //     }
                //     else
                //     {
                //         if(cur_token.type != TOKEN_TYPE_PUNCTUATOR || cur_token.token != PUNCTUATOR_RBRACE)
                //         {
                //             piss(PISS_ERROR, "Expecting ']' at line %d, column %d!", cur_token.line, cur_token.column);
                //         }

                //         next_token();
                //     }
                // }
            }

            if(!operand_index)
            {
                if(cur_token.type != TOKEN_TYPE_PUNCTUATOR || cur_token.token != PUNCTUATOR_COMMA)
                {
                    /* no comma after the first operand, so check if this instruction takes only one operand */
                    for(uint32_t index = 0; index < variant_count; index++)
                    {
                        if(variant_buffer[index].operands[1])
                        {
                            variant_buffer[index] = variant_buffer[variant_count - 1];
                            variant_count--;
                            index--;
                        }
                    }
                    
                    if(!variant_count)
                    {
                        piss(PISS_ERROR, "Missing second operand at line %d, column %d!", cur_token.line, cur_token.column);
                    }

                    break;
                }
                else
                {
                    /* we found a ',' */
                    next_token();
                }
            }
        }
    }

    uint32_t clear_mask = ~((1 << REG_CONST) | (1 << REG_INDIRECT));
    operands[0] &= clear_mask;
    operands[1] &= clear_mask;

    if((variant_buffer[0].opcode & WORD_OPCODE_MASK) == WORD_OPCODE_MASK)
    {
        variant_buffer[0].opcode |= (operands[1] | (operands[0] << 4)) << 8;
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


    // if(cur_token.type == TOKEN_TYPE_REG)
    // {
    //     /* first operand is a register */
    //     dst_reg = regs + cur_token.token;
    //     uint32_t reg_index = 1 << dst_reg->value;

    //     for(uint32_t index = 0; index < variant_count; index++)
    //     {
    //         if(!(variant_buffer[index].operands[0] & reg_index))
    //         {
    //             variant_buffer[index] = variant_buffer[variant_count - 1];
    //             variant_count--;
    //             index--;
    //         }
    //     }

    //     if(!variant_count)
    //     {
    //         piss(PISS_ERROR, "Invalid first operand at line %d, column %d!", cur_token.line, cur_token.column);
    //     }

    // }
    // else if(cur_token.type == TOKEN_TYPE_CONSTANT || cur_token.type == TOKEN_TYPE_NAME)
    // {
    //     /* first operand is a constant or a label */
    //     uint16_t constant = 0;
    //     uint32_t reg_index = 1 << REG_CONST;
    //     for(uint32_t index = 0; index < variant_count; index++)
    //     {
    //         if(!(variant_buffer[index].operands[0] & reg_index))
    //         {
    //             variant_buffer[index] = variant_buffer[variant_count - 1];
    //             variant_count--;
    //             index--;
    //         }
    //     }

    //     if(!variant_count)
    //     {
    //         piss(PISS_ERROR, "Invalid first operand at line %d, column %d!", cur_token.line, cur_token.column);
    //     }

    //     emit_opcode(variant_buffer[0].opcode);

    //     if(cur_token.type == TOKEN_TYPE_NAME)
    //     {
    //         struct label_t *label = find_label_from_token(&cur_token);

    //         if(label)
    //         {
    //             constant = label->offset;
    //         }
    //         else
    //         {
    //             create_patch(&cur_token, output_offset);
    //         }
    //     }
    //     else
    //     {
    //         constant = cur_token.token;
    //     }

    //     if(variant_buffer[0].width == 1)
    //     {
    //         emit_byte(constant);
    //     }
    //     else
    //     {
    //         emit_word(constant);
    //     }

    //     next_token();
    //     return;
    // }
    // // else if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LBRACE)
    // // {
    // //     /* first operand is a memory location */
    // // }
    // else
    // {
    //     /* check if this instruction takes no operands */
    //     uint16_t constant = 0;
    //     uint32_t reg_index = 1 << REG_CONST;
    //     for(uint32_t index = 0; index < variant_count; index++)
    //     {
    //         if(!(variant_buffer[index].operands[0] & reg_index))
    //         {
    //             variant_buffer[index] = variant_buffer[variant_count - 1];
    //             variant_count--;
    //             index--;
    //         }
    //     }

    //     if(!variant_count)
    //     {
    //         piss(PISS_ERROR, "Expecting operand at line %d, column %d!", cur_token.line, cur_token.column);
    //     }

    //     /* instruction take no operands, so we're done */
    //     emit_opcode(variant_buffer[0].opcode);
    //     return;
    // }

    // next_token();

    // if(cur_token.type != TOKEN_TYPE_PUNCTUATOR || cur_token.token != PUNCTUATOR_COMMA)
    // {
    //     /* check if this instruction takes only one operand */
    //     uint32_t index = 0;
    //     for(uint32_t index = 0; index < variant_count; index++)
    //     {
    //         if(variant_buffer[index].operands[1])
    //         {
    //             variant_buffer[index] = variant_buffer[variant_count - 1];
    //             variant_count--;
    //             index--;
    //         }
    //     }

    //     if(!variant_count)
    //     {
    //         piss(PISS_ERROR, "Missing second operand at line %d, column %d!", cur_token.line, cur_token.column);
    //     }

    //     /* instruction has no second operand, so we're done */
    //     emit_opcode(variant_buffer[0].opcode);
    //     return;
    // }

    // next_token();

    // if(cur_token.type == TOKEN_TYPE_REG)
    // {
    //     /* second operand is a register */
    //     src_reg = regs + cur_token.token;
    //     uint32_t reg_index = 1 << src_reg->value;

    //     for(uint32_t index = 0; index < variant_count; index++)
    //     {
    //         if(!(variant_buffer[index].operands[1] & reg_index))
    //         {
    //             variant_buffer[index] = variant_buffer[variant_count - 1];
    //             variant_count--;
    //             index--;
    //         }
    //     }
        
    //     if(!variant_count)
    //     {
    //         piss(PISS_ERROR, "Invalid second operand at line %d, column %d!", cur_token.line, cur_token.column);
    //     }

    //     emit_opcode(variant_buffer[0].opcode);
    // }
    // else if(cur_token.type == TOKEN_TYPE_CONSTANT || cur_token.type == TOKEN_TYPE_NAME)
    // {
    //     /* second operand is a constant or a label */
    //     uint32_t reg_index = 1 << REG_CONST;
    //     for(uint32_t index = 0; index < variant_count; index++)
    //     {
    //         if(!(variant_buffer[index].operands[1] & reg_index))
    //         {
    //             variant_buffer[index] = variant_buffer[variant_count - 1];
    //             variant_count--;
    //             index--;
    //         }
    //     }
        
    //     if(!variant_count)
    //     {
    //         piss(PISS_ERROR, "Invalid second operand at line %d, column %d!", cur_token.line, cur_token.column);
    //     }

    //     emit_opcode(variant_buffer[0].opcode);

    //     uint32_t constant = 0;

    //     if(cur_token.type == TOKEN_TYPE_NAME)
    //     {
    //         /* second operand is a label, so try finding it */
    //         struct label_t *label = find_label_from_token(&cur_token);

    //         if(label)
    //         {
    //             constant = label->offset;
    //         }
    //         else
    //         {
    //             /* didn't find a label, so create a patch so we know where to 
    //             update once all labels have been found */
    //             create_patch(&cur_token, output_offset);
    //         }
    //     }
    //     else
    //     {
    //         constant = cur_token.token;
    //     }

    //     if(variant_buffer[0].width == 1)
    //     {
    //         emit_byte(constant);
    //     }
    //     else
    //     {
    //         emit_word(constant);
    //     }
    // }
    // else if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LBRACE)
    // {
    //     /* second operand is a memory location */
    // }
    // else
    // {
    //     piss(PISS_ERROR, "Expecting operand at line %d, column %d!", cur_token.line, cur_token.column);
    // }

    // next_token();
}

void parse_label()
{
    struct token_t label_token = cur_token;
    next_token();
    if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_COLLON)
    {
        /* this is a label */
        struct label_t *label = create_label(&label_token, output_offset);
        next_token();
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

    next_token();

    if(cur_token.type != TOKEN_TYPE_KEYWORD)
    {
        piss(PISS_ERROR, "Expecting keyword on line %d, column %d!", cur_token.line, cur_token.column);
    }

    struct token_t keyword = cur_token;

    next_token();

    if(keyword.token == KEYWORD_STRING)
    {
        if(cur_token.type != TOKEN_TYPE_LITERAL)
        {
            piss(PISS_ERROR, "Expecting string literal at line %d, column %d!", cur_token.line, cur_token.column);
        }

        uint32_t offset = cur_token.token;
        for(uint32_t index = 0; index < cur_token.length; index++)
        {
            char c = source[offset + index];

            if(c == '\n')
            {
                emit_byte(' ');
            }
            else
            {
                emit_byte(c);
            }
        }
        emit_byte('\0');
        next_token();
        return;
    }
    else
    {
        uint32_t alloc_count = 1;

        if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LPARENTHESIS)
        {
            /* the keyword will receive a size "parameter" */
            next_token();
            struct token_t constant = cur_token;

            if(constant.type != TOKEN_TYPE_CONSTANT)
            {
                printf(PISS_ERROR, "Expecting constant on line %d, column %d!", cur_token.line, cur_token.column);
            }

            next_token();

            if(cur_token.type != TOKEN_TYPE_PUNCTUATOR || cur_token.token != PUNCTUATOR_RPARENTHESIS)
            {
                printf(PISS_ERROR, "Expecting ')' on line %d, column %d!", cur_token.line, cur_token.column);
            }

            alloc_count = constant.token;

            next_token();
        }

        if(keyword.token == KEYWORD_BYTE)
        {
            uint32_t count = 0;
            while(cur_token.type == TOKEN_TYPE_CONSTANT)
            {
                count++;
                emit_byte(cur_token.token);
                next_token();
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
                next_token();
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
    struct label_t *label = calloc(1, sizeof(struct label_t *));

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
    return create_label_on_list(source + token->token, token->length, offset, &labels, &last_label);
}

struct label_t *create_patch(struct token_t *token, uint16_t offset)
{
    return create_label_on_list(source + token->token, token->length, offset, &patches, &last_patch);
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