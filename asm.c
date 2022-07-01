#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "asm.h"


// struct keyword_t keywords[] = {
//     [KEYWORD_MOV]   = {.text = "mov", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_ADD]   = {.text = "add", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_SUB]   = {.text = "sub", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_MUL]   = {.text = "mul", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_DIV]   = {.text = "div", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_CMP]   = {.text = "cmp", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_AND]   = {.text = "and", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_OR]    = {.text = "or", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_XOR]   = {.text = "xor", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_NOT]   = {.text = "not", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_SHL]   = {.text = "shl", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_SHR]   = {.text = "shr", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_JMP]   = {.text = "jmp", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_JZ]    = {.text = "jz", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_JNZ]   = {.text = "jnz", .type = KEYWORD_TYPE_INSTRUCTION},
//     [KEYWORD_JE]    = {.text = "je", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_JNE]   = {.text = "jne", .type = KEYWORD_TYPE_INSTRUCTION },
//     [KEYWORD_CALL]  = {.text = "call", .type = KEYWORD_TYPE_INSTRUCTION},

//     [KEYWORD_ACCL]  = {.text = "accl", .type = KEYWORD_TYPE_REGISTER },
//     [KEYWORD_ACCH]  = {.text = "acch", .type = KEYWORD_TYPE_REGISTER },
//     [KEYWORD_ACC]   = {.text = "acc", .type = KEYWORD_TYPE_REGISTER },
//     [KEYWORD_BASE]  = {.text = "base", .type = KEYWORD_TYPE_REGISTER },
//     [KEYWORD_STT]    = {.text = "stt", .type = KEYWORD_TYPE_REGISTER },
//     [KEYWORD_STB]    = {.text = "stb", .type = KEYWORD_TYPE_REGISTER }
// };

struct reg_t regs[] = {
    {.name = "accl", .value = REG_ACCL},
    {.name = "acch", .value = REG_ACCH},
    {.name = "acc", .value = REG_ACCW},
    {.name = "accw", .value = REG_ACCW},
    {.name = "base", .value = REG_BASE},
    {.name = "stt", .value = REG_STT},
    {.name = "stb", .value = REG_STB},
};

struct opcode_t opcodes[] = {
    [OPCODE_MOV]    = {
        .name = "mov",
        .variant_count = 17,
        .variants = (struct opvariant_t []) {
            {.opcode = 0x01, .dst_reg = 1 << REG_ACCL},
            {.opcode = 0x02, .dst_reg = 1 << REG_ACCL, .src_reg = 1 << REG_ACCH},

            {.opcode = 0x03, .dst_reg = 1 << REG_ACCH},
            {.opcode = 0x04, .dst_reg = 1 << REG_ACCH, .src_reg = 1 << REG_ACCL},

            {.opcode = 0x05, .dst_reg = 1 << REG_ACCW},
            {.opcode = 0x06, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_BASE},
            {.opcode = 0x07, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_STT},
            {.opcode = 0x08, .dst_reg = 1 << REG_ACCW, .src_reg = 1 << REG_STB},

            {.opcode = 0x09, .dst_reg = 1 << REG_BASE},
            {.opcode = 0x0a, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_ACCW},
            {.opcode = 0x0b, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_STT},
            {.opcode = 0x0c, .dst_reg = 1 << REG_BASE, .src_reg = 1 << REG_STB},

            {.opcode = 0x0d, .dst_reg = 1 << REG_STT},
            {.opcode = 0x0e, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_ACCW},
            {.opcode = 0x0f, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_BASE},
            {.opcode = 0x10, .dst_reg = 1 << REG_STT, .src_reg = 1 << REG_STB},
            
            {.opcode = 0x11, .dst_reg = 1 << REG_STB, .src_reg = 1 << REG_STT},
        }
    },
    [OPCODE_ADD]    = {.name = "add"},
    [OPCODE_SUB]    = {.name = "sub"},
    [OPCODE_MUL]    = {.name = "mul"},
    [OPCODE_DIV]    = {.name = "div"},
    [OPCODE_CMP]    = {.name = "cmp"},
    [OPCODE_AND]    = {.name = "and"},
    [OPCODE_OR]     = {.name = "or"},
    [OPCODE_XOR]    = {.name = "xor"},
    [OPCODE_NOT]    = {.name = "not"},
    [OPCODE_SHL]    = {.name = "shl"},
    [OPCODE_SHR]    = {.name = "shr"},
    [OPCODE_INC]    = {.name = "inc"},
    [OPCODE_DEC]    = {.name = "dec"},
    [OPCODE_JMP]    = {.name = "jmp"},
    [OPCODE_JZ]     = {.name = "jz"},
    [OPCODE_JNZ]    = {.name = "jnz"},
    [OPCODE_CALL]   = {.name = "call"},
    [OPCODE_RET]    = {.name = "ret"}
};

const char punctuators[] = {
    [PUNCTUATOR_COMMA]      = ',',
    [PUNCTUATOR_LBRACE]     = '[',
    [PUNCTUATOR_RBRACE]     = ']',
    [PUNCTUATOR_PLUS]       = '+',
    [PUNCTUATOR_MINUS]      = '-'
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
    [','] = CHAR_TYPE_PUNCTUATOR
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
        while(source_offset < source_len && token_text_cursor < sizeof(token_text) && 
                map[source[source_offset]] != TOKEN_TYPE_SPACE && map[source[source_offset]] != TOKEN_TYPE_PUNCTUATOR)
        {
            token_text[token_text_cursor] = source[source_offset];

            if(!token_text_cursor)
            {
                if(map[token_text[token_text_cursor]] == TOKEN_TYPE_CONSTANT)
                {
                    maybe_constant = 1;
                }
            }
            else if(maybe_constant)
            {
                if(token_text_cursor == 1)
                {
                    if(token_text[token_text_cursor] == 'b' || token_text[token_text_cursor] == 'B')
                    {
                        constant_type = CONSTANT_BINARY;
                    }
                    else if(token_text[token_text_cursor] == 'x' || token_text[token_text_cursor] == 'X')
                    {
                        constant_type = CONSTANT_HEXADECIMAL;
                    }
                    else if(map[token_text[token_text_cursor]] == TOKEN_TYPE_CONSTANT)
                    {
                        constant_type = CONSTANT_DECIMAL;
                    }
                    else
                    {
                        piss(PISS_ERROR, "Uknown constant type one line %d, column %d!", line, column);
                    }
                }
            }

            source_offset++;
            token_text_cursor++;
            column++;
        }

        token_text[token_text_cursor] = '\0';

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
            cur_token.line = line;
            cur_token.column = column;
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
                    cur_token.line = line;
                    cur_token.column = column;
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
                    cur_token.line = line;
                    cur_token.column = column;
                    return;
                }
            }

            /* identifiers/labels would be handled here */
            piss(PISS_ERROR, "Unknown keyword [%s] at line %d, column %d!", token_text, start_line, start_column);
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

        // if(cur_token.type == TOKEN_TYPE_KEYWORD && keywords[cur_token.token].type == KEYWORD_TYPE_INSTRUCTION)
        // {
            
        // }

        // switch(cur_token.type)
        // {
        //     case TOKEN_TYPE_OPCODE:
        //         printf("opcode: %s\n", opcodes[cur_token.token].name);
        //     break;

        //     case TOKEN_TYPE_REG:
        //         printf("reg: %s\n", regs[cur_token.token].name);
        //     break;

        //     case TOKEN_TYPE_PUNCTUATOR:
        //         printf("punctuator: %c\n", punctuators[cur_token.token]);
        //     break;

        //     case TOKEN_TYPE_CONSTANT:
        //         printf("constant: %x\n", cur_token.token);
        //     break;
        // }
    }
}

void parse_instruction()
{
    struct opcode_t *opcode = opcodes + cur_token.token;
    struct reg_t *src_reg = NULL;
    struct reg_t *dst_reg = NULL;
    uint32_t single_operand = 0;
    next_token();

    if(cur_token.type == TOKEN_TYPE_REG)
    {
        /* instruction destination is a register */
        // dst_reg = cur_token;
        dst_reg = regs + cur_token.token;
    }
    else if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LBRACE)
    {
        /* instruction destination is a memory location */
    }
    else
    {
        piss(PISS_ERROR, "Expecting register or '[' at line %d, column %d!", cur_token.line, cur_token.column);
    }

    next_token();

    if(cur_token.type != TOKEN_TYPE_PUNCTUATOR)
    {
        /* check if this instruction takes only one operand */
        uint32_t index = 0;

        for(; index < opcode->variant_count; index++)
        {
            if(opcode->variants[index].src_reg == 0)
            {
                break;
            }
        }

        if(index >= opcode->variant_count)
        {
            piss(PISS_ERROR, "Missing second operand at line %d, column %d!", cur_token.line, cur_token.column);
        }

        single_operand = 1;
    }
    else if(cur_token.token != PUNCTUATOR_COMMA)
    {
        /* the only token permitted after the first operand is a comma, so if we got anything else here, that's bad */
        piss(PISS_ERROR, "Expecting ',' at line %d, column %d!", cur_token.line, cur_token.column);
    }

    if(single_operand)
    {
        /* we're done here, so emit an instruction */
        printf("opcode: %s, operand: %s\n", opcode->name, dst_reg->name);
        return;
    }

    next_token();

    if(cur_token.type == TOKEN_TYPE_REG)
    {
        /* second operand is a register */
        src_reg = regs + cur_token.token;
        printf("opcode: %s, first operand: %s, second operand: %s\n", opcode->name, dst_reg->name, src_reg->name);
    }
    else if(cur_token.type == TOKEN_TYPE_CONSTANT)
    {
        /* second operand is a constant */
        printf("opcode: %s, first operand: %s, second operand: %x\n", opcode->name, dst_reg->name, cur_token.token);
    }
    else if(cur_token.type == TOKEN_TYPE_PUNCTUATOR && cur_token.token == PUNCTUATOR_LBRACE)
    {

    }
    else
    {
        piss(PISS_ERROR, "Expecting register or '[' at line %d, column %d!", cur_token.line, cur_token.column);
    }
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

        return 0;
    }

    piss(PISS_ERROR, "No arguments provided!");
    return 0;
}