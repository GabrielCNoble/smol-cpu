#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "asm.h"


struct keyword_t keywords[] = {
    [KEYWORD_MOV]   = {.text = "mov", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_ADD]   = {.text = "add", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_SUB]   = {.text = "sub", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_MUL]   = {.text = "mul", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_DIV]   = {.text = "div", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_CMP]   = {.text = "cmp", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_AND]   = {.text = "and", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_OR]    = {.text = "or", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_XOR]   = {.text = "xor", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_NOT]   = {.text = "not", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_SHL]   = {.text = "shl", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_SHR]   = {.text = "shr", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_JMP]   = {.text = "jmp", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_JZ]    = {.text = "jz", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_JE]    = {.text = "je", .type = KEYWORD_TYPE_INSTRUCTION },
    [KEYWORD_JNE]   = {.text = "jne", .type = KEYWORD_TYPE_INSTRUCTION },

    [KEYWORD_ACCL]  = {.text = "accl", .type = KEYWORD_TYPE_REGISTER },
    [KEYWORD_ACCH]  = {.text = "acch", .type = KEYWORD_TYPE_REGISTER },
    [KEYWORD_ACC]   = {.text = "acc", .type = KEYWORD_TYPE_REGISTER },
    [KEYWORD_BASE]  = {.text = "base", .type = KEYWORD_TYPE_REGISTER },
    [KEYWORD_STT]    = {.text = "stt", .type = KEYWORD_TYPE_REGISTER },
    [KEYWORD_STB]    = {.text = "stb", .type = KEYWORD_TYPE_REGISTER }
};

// uint32_t instructions[] = {
//     [KEYWORD_MOV]
// };

const char punctuators[] = {
    [PUNCTUATOR_COMMA]      = ',',
    [PUNCTUATOR_LBRACE]     = '[',
    [PUNCTUATOR_RBRACE]     = ']',
    [PUNCTUATOR_PLUS]       = '+',
    [PUNCTUATOR_MINUS]      = '-'
};


const char map[] = {
    ['a'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['b'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['c'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['d'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['e'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['f'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['g'] = TOKEN_TYPE_KEYWORD,
    ['h'] = TOKEN_TYPE_KEYWORD,
    ['i'] = TOKEN_TYPE_KEYWORD,
    ['j'] = TOKEN_TYPE_KEYWORD,
    ['k'] = TOKEN_TYPE_KEYWORD,
    ['l'] = TOKEN_TYPE_KEYWORD,
    ['m'] = TOKEN_TYPE_KEYWORD,
    ['n'] = TOKEN_TYPE_KEYWORD,
    ['o'] = TOKEN_TYPE_KEYWORD,
    ['p'] = TOKEN_TYPE_KEYWORD,
    ['q'] = TOKEN_TYPE_KEYWORD,
    ['r'] = TOKEN_TYPE_KEYWORD,
    ['s'] = TOKEN_TYPE_KEYWORD,
    ['t'] = TOKEN_TYPE_KEYWORD,
    ['u'] = TOKEN_TYPE_KEYWORD,
    ['v'] = TOKEN_TYPE_KEYWORD,
    ['w'] = TOKEN_TYPE_KEYWORD,
    ['x'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['y'] = TOKEN_TYPE_KEYWORD,
    ['z'] = TOKEN_TYPE_KEYWORD,

    ['A'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['B'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['C'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['D'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['E'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['F'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['G'] = TOKEN_TYPE_KEYWORD,
    ['H'] = TOKEN_TYPE_KEYWORD,
    ['I'] = TOKEN_TYPE_KEYWORD,
    ['J'] = TOKEN_TYPE_KEYWORD,
    ['K'] = TOKEN_TYPE_KEYWORD,
    ['L'] = TOKEN_TYPE_KEYWORD,
    ['M'] = TOKEN_TYPE_KEYWORD,
    ['N'] = TOKEN_TYPE_KEYWORD,
    ['O'] = TOKEN_TYPE_KEYWORD,
    ['P'] = TOKEN_TYPE_KEYWORD,
    ['Q'] = TOKEN_TYPE_KEYWORD,
    ['R'] = TOKEN_TYPE_KEYWORD,
    ['S'] = TOKEN_TYPE_KEYWORD,
    ['T'] = TOKEN_TYPE_KEYWORD,
    ['Y'] = TOKEN_TYPE_KEYWORD,
    ['V'] = TOKEN_TYPE_KEYWORD,
    ['W'] = TOKEN_TYPE_KEYWORD,
    ['X'] = TOKEN_TYPE_CONSTANT | TOKEN_TYPE_KEYWORD,
    ['Y'] = TOKEN_TYPE_KEYWORD,
    ['Z'] = TOKEN_TYPE_KEYWORD,

    ['0'] = TOKEN_TYPE_CONSTANT,
    ['1'] = TOKEN_TYPE_CONSTANT,
    ['2'] = TOKEN_TYPE_CONSTANT,
    ['3'] = TOKEN_TYPE_CONSTANT,
    ['4'] = TOKEN_TYPE_CONSTANT,
    ['5'] = TOKEN_TYPE_CONSTANT,
    ['6'] = TOKEN_TYPE_CONSTANT,
    ['7'] = TOKEN_TYPE_CONSTANT,
    ['8'] = TOKEN_TYPE_CONSTANT,
    ['9'] = TOKEN_TYPE_CONSTANT,

    ['+'] = TOKEN_TYPE_PUNCTUATOR,
    ['-'] = TOKEN_TYPE_PUNCTUATOR,
    ['['] = TOKEN_TYPE_PUNCTUATOR,
    [']'] = TOKEN_TYPE_PUNCTUATOR,
    [','] = TOKEN_TYPE_PUNCTUATOR
};

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
            column = 1;
            line++;
        }

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
        }
        else
        {
            /* probably a keyword, so find out which */
            uint32_t keyword = 0;

            for(; keyword < KEYWORD_LAST; keyword++)
            {
                if(!strcmp(token_text, keywords[keyword].text))
                {
                    cur_token.type = TOKEN_TYPE_KEYWORD;
                    cur_token.token = keyword;
                    break;
                }
            }

            if(keyword == KEYWORD_LAST)
            {
                /* identifiers would be handled here */
                piss(PISS_ERROR, "Unknown keyword [%s] at line %d, column %d!", token_text, start_line, start_column);
            }
        }
    }
}

void parse()
{
    while(source_offset < source_len)
    {
        next_token();

        if(cur_token.type == TOKEN_TYPE_KEYWORD && keywords[cur_token.token].type == KEYWORD_TYPE_INSTRUCTION)
        {
            
        }

        // switch(cur_token.type)
        // {
        //     case TOKEN_TYPE_KEYWORD:
        //         printf("keyword: %s\n", keywords[cur_token.token].text);
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