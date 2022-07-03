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
        .variant_count = 17,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x01, .width = 1, .dst_reg = 1 << REG_ACCL, .src_reg = 0xffff},
            {.opcode = 0x02, .width = 1, .dst_reg = 1 << REG_ACCL, .src_reg = 1 << REG_ACCH},

            {.opcode = 0x03, .width = 2, .dst_reg = 1 << REG_ACCH, .src_reg = 0xffff},
            {.opcode = 0x04, .width = 2, .dst_reg = 1 << REG_ACCH, .src_reg = 1 << REG_ACCL},

            {.opcode = 0x05, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 0xffff},
            {.opcode = 0x06, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_BASE},
            {.opcode = 0x07, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_STT},
            {.opcode = 0x08, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_STB},

            {.opcode = 0x09, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 0xffff},
            {.opcode = 0x0a, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_ACCW},
            {.opcode = 0x0b, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_STT},
            {.opcode = 0x0c, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_STB},

            {.opcode = 0x0d, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 0xffff},
            {.opcode = 0x0e, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_ACCW},
            {.opcode = 0x0f, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_BASE},
            {.opcode = 0x10, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_STB},
            
            {.opcode = 0x11, .width = 2, .dst_reg = 1 << REG_STB, .src_reg = 1 << REG_STT},
        }
    },
    [OPCODE_ADD]    = {
        .name = "add",
        .variant_count = 15,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x12, .width = 1, .dst_reg = 1 << REG_ACCL, .src_reg = 0xffff},
            {.opcode = 0x13, .width = 1, .dst_reg = 1 << REG_ACCL, .src_reg = 1 << REG_ACCH},

            {.opcode = 0x14, .width = 1, .dst_reg = 1 << REG_ACCH, .src_reg = 0xffff},
            {.opcode = 0x15, .width = 1, .dst_reg = 1 << REG_ACCH, .src_reg = 1 << REG_ACCL},

            {.opcode = 0x16, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 0xffff},
            {.opcode = 0x17, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_BASE},
            {.opcode = 0x18, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_STT},
            {.opcode = 0x19, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_STB},

            {.opcode = 0x1a, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 0xffff},
            {.opcode = 0x1b, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_ACCW},
            {.opcode = 0x1c, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_STT},
            {.opcode = 0x1d, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_STB},

            {.opcode = 0x1e, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 0xffff},
            {.opcode = 0x1f, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_ACCW},
            {.opcode = 0x20, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_BASE},
        }
    },
    [OPCODE_SUB]    = {
        .name = "sub",
        .variant_count = 15,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x21, .width = 1, .dst_reg = 1 << REG_ACCL, .src_reg = 0xffff},
            {.opcode = 0x22, .width = 1, .dst_reg = 1 << REG_ACCL, .src_reg = 1 << REG_ACCH},

            {.opcode = 0x23, .width = 1, .dst_reg = 1 << REG_ACCH, .src_reg = 0xffff},
            {.opcode = 0x24, .width = 1, .dst_reg = 1 << REG_ACCH, .src_reg = 1 << REG_ACCL},

            {.opcode = 0x25, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 0xffff},
            {.opcode = 0x26, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_BASE},
            {.opcode = 0x27, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_STT},
            {.opcode = 0x28, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_STB},

            {.opcode = 0x29, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 0xffff},
            {.opcode = 0x2a, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_ACCW},
            {.opcode = 0x2b, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_STT},
            {.opcode = 0x2c, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_STB},

            {.opcode = 0x2d, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 0xffff},
            {.opcode = 0x2e, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_ACCW},
            {.opcode = 0x2f, .width = 2, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_BASE},
        }
    },
    [OPCODE_MUL]    = {.name = "mul"},
    [OPCODE_DIV]    = {.name = "div"},
    [OPCODE_CMP]    = {
        .name = "cmp",
        .variant_count = 8,
        .variants = (struct opvariant_t[]) {
            {.opcode = 0x3b, .width = 1, .dst_reg = 1 << REG_ACCL, .src_reg = 0xffff},
            {.opcode = 0x3c, .width = 1, .dst_reg = 1 << REG_ACCL, .src_reg = 1 << REG_ACCH},

            {.opcode = 0x3d, .width = 1, .dst_reg = 1 << REG_ACCH, .src_reg = 0xffff},
            {.opcode = 0x3e, .width = 1, .dst_reg = 1 << REG_ACCH, .src_reg = 1 << REG_ACCH},

            {.opcode = 0x3f, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 0xffff},
            {.opcode = 0x40, .width = 2, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_BASE},

            {.opcode = 0x41, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 0xffff},
            {.opcode = 0x42, .width = 2, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_ACCW}
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
            {.opcode = 0x77, .width = 2, .dst_reg = 0xffff}
        }
    },
    [OPCODE_JZ]     = {
        .name = "jz",
        .variant_count = 1,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x78, .width = 2, .dst_reg = 0xffff}
        }
    },
    [OPCODE_JNZ]    = {.name = "jnz"},
    [OPCODE_CALL]   = {.name = "call"},
    [OPCODE_RET]    = {.name = "ret"},
    [OPCODE_PUSH]   = {.name = "push"},
    [OPCODE_POP]    = {.name = "pop"}
};

struct opvariant_t variant_buffer[MAX_OPCODE_VARIANTS];

const char punctuators[] = {
    [PUNCTUATOR_COMMA]      = ',',
    [PUNCTUATOR_COLLON]     = ':',
    [PUNCTUATOR_LBRACE]     = '[',
    [PUNCTUATOR_RBRACE]     = ']',
    [PUNCTUATOR_PLUS]       = '+',
    [PUNCTUATOR_MINUS]      = '-',
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

    ['+'] = CHAR_TYPE_PUNCTUATOR,
    ['-'] = CHAR_TYPE_PUNCTUATOR,
    ['['] = CHAR_TYPE_PUNCTUATOR,
    [']'] = CHAR_TYPE_PUNCTUATOR,
    [','] = CHAR_TYPE_PUNCTUATOR,
    [':'] = CHAR_TYPE_PUNCTUATOR
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

struct code_buffer_t *output_buffer = NULL;
struct code_buffer_t *cur_buffer = NULL;
uint32_t output_offset = 0;

struct label_t *labels = NULL;
struct label_t *last_label = NULL;

struct label_t *patches = NULL;
struct label_t *last_patch = NULL;

void next_token()
{
    char token_text[64];
    uint32_t token_text_cursor = 0;

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

    if(map[source[source_offset]] == TOKEN_TYPE_PUNCTUATOR)
    {
        cur_token.type = TOKEN_TYPE_PUNCTUATOR;

        switch(source[source_offset])
        {
            case ',':
                cur_token.token = PUNCTUATOR_COMMA;        
            break;

            case '[':
                cur_token.token = PUNCTUATOR_LBRACE;
            break;

            case ']':
                cur_token.token = PUNCTUATOR_RBRACE;
            break;

            case '+':
                cur_token.token = PUNCTUATOR_PLUS;
            break;

            case '-':
                cur_token.token = PUNCTUATOR_MINUS;
            break;

            case ':':
                cur_token.token = PUNCTUATOR_COLLON;
            break;
        }

        cur_token.line = line;
        cur_token.column = column;

        source_offset++;
        column++;
    }
    else
    {
        uint32_t maybe_constant = 0;
        uint32_t constant_type = CONSTANT_NONE;
        uint32_t start_column = column;
        uint32_t start_line = line;
        uint32_t start_offset = source_offset;
        while(source_offset < source_len && token_text_cursor < sizeof(token_text) && 
                map[source[source_offset]] != CHAR_TYPE_SPACE && map[source[source_offset]] != CHAR_TYPE_PUNCTUATOR)
        {
            token_text[token_text_cursor] = source[source_offset];

            // if(!token_text_cursor)
            // {
            //     if(map[token_text[token_text_cursor]] & CHAR_TYPE_CONSTANT)
            //     {
            //         maybe_constant = 1;
            //     }
            // }
            // else if(maybe_constant)
            // {
            //     if(token_text_cursor == 1)
            //     {
            //         if(token_text[token_text_cursor] == 'b' || token_text[token_text_cursor] == 'B')
            //         {
            //             constant_type = CONSTANT_BINARY;
            //         }
            //         else if(token_text[token_text_cursor] == 'x' || token_text[token_text_cursor] == 'X')
            //         {
            //             constant_type = CONSTANT_HEXADECIMAL;
            //         }
            //         else if((map[token_text[token_text_cursor]] & CHAR_TYPE_CONSTANT) && 
            //                 map[token_text[token_text_cursor - 1]] == CHAR_TYPE_CONSTANT)
            //         {
            //             constant_type = CONSTANT_DECIMAL;
            //         }
            //         else
            //         {
            //             maybe_constant = 0;
            //         }
            //     }
            //     else
            //     {
            //         if(!(map[token_text[token_text_cursor]] & CHAR_TYPE_CONSTANT))
            //         {
            //             piss(PISS_ERROR, "Uknown constant type one line %d, column %d!", start_line, start_column);
            //         }
            //     }
            // }

            source_offset++;
            token_text_cursor++;
            column++;
        }

        token_text[token_text_cursor] = '\0';
        cur_token.line = start_line;
        cur_token.column = start_column;

        if(map[token_text[0]] & CHAR_TYPE_CONSTANT)
        {
            if(token_text[0] == '0' && (token_text[1] == 'b' || token_text[1] == 'B') && token_text_cursor > 2)
            {
                /* binary constants starts with a 0b, and may be valid only if there's at least one character after
                the prefix */
                constant_type = CONSTANT_BINARY;
            }
            else if(token_text[0] == '0' && (token_text[1] == 'x' || token_text[1] == 'X') && token_text_cursor > 2)
            {
                /* hex constants starts with a 0x, and may be valid only if there's at least one character after
                the prefix */
                constant_type = CONSTANT_HEXADECIMAL;
            }
            else if(map[token_text[0]] == CHAR_TYPE_CONSTANT)
            {
                constant_type = CONSTANT_DECIMAL;
            }
            
            if(constant_type != CONSTANT_NONE)
            {
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

                case CONSTANT_BINARY:
                    /* get rid of the 0b prefix, since strtol won't accept it */
                    token_text[0] = ' ';
                    token_text[1] = ' ';
                    constant_value = strtol(token_text, NULL, 2);
                break;
            }

            if(constant_value > 0xffff)
            {
                piss(PISS_ERROR, "Constant out of range at line %d, column %d!", line, column);
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
                    return;
                }
            }
            
            /* test all regs */
            for(uint32_t index = 0; index < sizeof(regs) / sizeof(regs[0]); index++)
            {
                if(!strcmp(token_text, regs[index].name))
                {
                    cur_token.type = TOKEN_TYPE_REG;
                    cur_token.token = index;
                    return;
                }
            }

            cur_token.type = TOKEN_TYPE_NAME;
            cur_token.token = start_offset;
            cur_token.length = token_text_cursor;
            // printf("%c\n", source[start_column]);
            return;

            /* identifiers/labels would be handled here */
            // piss(PISS_ERROR, "Unknown keyword [%s] at line %d, column %d!", token_text, start_line, start_column);
        }
    }
}

void parse()
{
    while(source_offset < source_len)
    {
        next_token();

        if(cur_token.type == TOKEN_TYPE_OPCODE)
        {
            
            parse_instruction();
        }
        else if(cur_token.type == TOKEN_TYPE_NAME)
        {
            parse_label();
        }
    }
}

void parse_instruction()
{
    uint32_t opcode_index = cur_token.token;
    struct opcode_t *opcode = opcodes + opcode_index;
    struct reg_t *src_reg = NULL;

    struct reg_t *dst_reg = NULL;
    uint32_t single_operand = 0;
    // printf("%x\n", cur_token.type);
    next_token();

    memcpy(variant_buffer, opcode->variants, sizeof(struct opvariant_t) * opcode->variant_count);
    uint32_t variant_count = opcode->variant_count;

    // printf("%x\n", cur_token.type);

    if(cur_token.type == TOKEN_TYPE_REG)
    {
        /* first operand is a register */
        dst_reg = regs + cur_token.token;
        uint16_t reg_index = 1 << dst_reg->value;

        for(uint32_t index = 0; index < variant_count; index++)
        {
            if(!(variant_buffer[index].dst_reg & reg_index) && variant_count)
            {
                variant_buffer[index] = variant_buffer[variant_count - 1];
                variant_count--;
                index--;
            }
        }

    }
    else if(cur_token.type == TOKEN_TYPE_CONSTANT || cur_token.type == TOKEN_TYPE_NAME)
    {
        /* first operand is a constant or a label */
        uint16_t constant = 0;

        for(uint32_t index = 0; index < variant_count; index++)
        {
            if(variant_buffer[index].dst_reg != 0xffff && variant_count)
            {
                variant_buffer[index] = variant_buffer[variant_count - 1];
                variant_count--;
                index--;
            }
        }

        if(!variant_count)
        {
            piss(PISS_ERROR, "Invalid first operand at line %d, column %d!", cur_token.line, cur_token.column);
        }

        emit_opcode(variant_buffer[0].opcode);

        if(cur_token.type == TOKEN_TYPE_NAME)
        {
            struct label_t *label = find_label_from_token(&cur_token);

            if(label)
            {
                constant = label->offset;
            }
            else
            {
                create_patch(&cur_token, output_offset);
            }
        }
        else
        {
            constant = cur_token.token;
        }

        if(variant_buffer[0].width == 1)
        {
            emit_byte(constant);
        }
        else
        {
            emit_word(constant);
        }

        return;
    }
    else if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LBRACE)
    {
        /* first operand is a memory location */
    }
    else
    {
        piss(PISS_ERROR, "Expecting operand at line %d, column %d!", cur_token.line, cur_token.column);
    }

    next_token();

    if(cur_token.type != TOKEN_TYPE_PUNCTUATOR)
    {
        /* check if this instruction takes only one operand */
        uint32_t index = 0;

        for(uint32_t index = 0; index < variant_count; index++)
        {
            if(variant_buffer[index].src_reg && variant_count)
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

        /* instruction has no second operand, so we're done */
        emit_opcode(variant_buffer[0].opcode);
        return;
    }
    else if(cur_token.token != PUNCTUATOR_COMMA)
    {
        /* the only token permitted after the first operand is a comma, so if we got anything else here, that's bad */
        piss(PISS_ERROR, "Expecting ',' at line %d, column %d!", cur_token.line, cur_token.column);
    }

    next_token();

    if(cur_token.type == TOKEN_TYPE_REG)
    {
        /* second operand is a register */
        src_reg = regs + cur_token.token;
        uint32_t reg_index = 1 << src_reg->value;

        for(uint32_t index = 0; index < variant_count; index++)
        {
            if(!(variant_buffer[index].src_reg & reg_index) && variant_count)
            {
                variant_buffer[index] = variant_buffer[variant_count - 1];
                variant_count--;
                index--;
            }
        }
        
        if(!variant_count)
        {
            piss(PISS_ERROR, "Invalid second operand at line %d, column %d!", cur_token.line, cur_token.column);
        }

        emit_opcode(variant_buffer[0].opcode);
    }
    else if(cur_token.type == TOKEN_TYPE_CONSTANT)
    {
        /* second operand is a constant */

        for(uint32_t index = 0; index < variant_count; index++)
        {
            if(variant_buffer[index].src_reg != 0xffff && variant_count)
            {
                variant_buffer[index] = variant_buffer[variant_count - 1];
                variant_count--;
                index--;
            }
        }
        
        if(!variant_count)
        {
            piss(PISS_ERROR, "Invalid second operand at line %d, column %d!", cur_token.line, cur_token.column);
        }

        emit_opcode(variant_buffer[0].opcode);

        if(variant_buffer[0].width == 1)
        {
            emit_byte(cur_token.token);
        }
        else
        {
            emit_word(cur_token.token);
        }
    }
    else if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LBRACE)
    {

    }
    else
    {
        piss(PISS_ERROR, "Expecting operand at line %d, column %d!", cur_token.line, cur_token.column);
    }
}

void parse_label()
{
    struct token_t label_token = cur_token;
    next_token();
    if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_COLLON)
    {
        /* this is a label */
        struct label_t *label = create_label(&label_token, output_offset);
    }
}

void emit_byte(uint8_t byte)
{
    if(!output_buffer)
    {
        output_buffer = calloc(1, sizeof(struct code_buffer_t));
        output_buffer->data = calloc(1, CODE_BUFFER_SIZE);
        cur_buffer = output_buffer;
    }

    cur_buffer->data[cur_buffer->offset] = byte;
    cur_buffer->offset++;

    if(cur_buffer->offset == CODE_BUFFER_SIZE)
    {
        struct code_buffer_t *new_buffer = calloc(1, sizeof(struct code_buffer_t));
        new_buffer->data = calloc(1, CODE_BUFFER_SIZE);
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
    struct code_buffer_t *code_buffer = output_buffer;
    uint32_t code_offset = CODE_BUFFER_SIZE;

    struct label_t *patch = patches;

    while(patch)
    {
        struct label_t *label = find_label(patch->name);

        if(!label)
        {
            piss(PISS_ERROR, "Undefined label %s!", patch->name);
        }

        while(code_offset < patch->offset)
        {
            code_offset += code_buffer->offset;
            code_buffer = code_buffer->next;
        }

        uint32_t patch_offset = patch->offset % CODE_BUFFER_SIZE;
        code_buffer->data[patch_offset] = (uint8_t)label->offset;
        code_buffer->data[patch_offset + 1] = (uint8_t)(label->offset >> 8);

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
            printf("MESASGE: ");
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
            struct code_buffer_t *buffer = output_buffer;

            while(buffer)
            {
                fwrite(buffer->data, 1, buffer->offset, output_file);
                buffer = buffer->next;
            }

            fclose(output_file);

            struct label_t *label = labels;
            while(label)
            {
                struct label_t *next_label = label->next;
                free(label->name);
                free(label);
                label = next_label;
            }

            buffer = output_buffer;
            while(buffer)
            {
                struct code_buffer_t *next_buffer = buffer->next;
                free(buffer->data);
                free(buffer);
                buffer = next_buffer;
            }
        }
        else
        {
            piss(PISS_ERROR, "Input file empty!");
        }

        return 0;
    }

    piss(PISS_ERROR, "No arguments provided!");
    return 0;
}