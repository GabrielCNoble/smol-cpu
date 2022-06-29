#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "asm.h"


const char *keywords[] = {
    [KEYWORD_MOV]   = "mov",
    [KEYWORD_ADD]   = "add",
    [KEYWORD_SUB]   = "sub",
    [KEYWORD_MUL]   = "mul",
    [KEYWORD_DIV]   = "div",
    [KEYWORD_CMP]   = "cmp",
    [KEYWORD_AND]   = "and",
    [KEYWORD_OR]    = "or",
    [KEYWORD_XOR]   = "xor",
    [KEYWORD_NOT]   = "not",
    [KEYWORD_SHL]   = "shl",
    [KEYWORD_SHR]   = "shr",
    [KEYWORD_JMP]   = "jmp",
    [KEYWORD_JZ]    = "jz",
    [KEYWORD_JE]    = "je",
    [KEYWORD_JNE]   = "jne",

    [KEYWORD_ACCL]  = "accl",
    [KEYWORD_ACCH]  = "acch",
    [KEYWORD_ACC]   = "acc",
};

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
};

uint32_t line = 0;
uint32_t column = 0;
uint32_t source_offset = 0;
uint32_t source_len = 0;
const char *source = NULL;

struct token_t cur_token = {
    .type = TOKEN_TYPE_SPACE
};


void next_token()
{
    while(source_offset < source_len && map[source[source_offset]] == TOKEN_TYPE_SPACE)
    {
        source_offset++;
    }

    if(map[source[source_offset]] == TOKEN_TYPE_SPACE)
    {
        piss(PISS_ERROR, "End of file reached!");
    }

    // if(map[source[source_offset]] & TOKEN_TYPE_)
}

void parse()
{

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



    }

    piss(PISS_ERROR, "No arguments provided!");
    return 0;
}