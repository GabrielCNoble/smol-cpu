#ifndef ASM_H
#define ASM_H

#include <stdint.h>

enum PISS_LEVEL
{
    PISS_ERROR = 0,
    PISS_WARNING,
    PISS_MESSAGE,
};

enum OPCODES
{
    OPCODE_NOP                  = 0x00,
    OPCODE_MOV_ACCL_IMM8        = 0x01,
    OPCODE_MOV_ACCH_IMM8        = 0x02,
    OPCODE_MOV_ACC_IMM16        = 0x03,

    OPCODE_ADD_ACCL_IMM8        = 0x05,
    OPCODE_ADD_ACCH_IMM8        = 0x06,
};

enum TOKEN_TYPE
{
    TOKEN_TYPE_PUNCTUATOR       = 1,
    TOKEN_TYPE_KEYWORD          = 1 << 1,
    TOKEN_TYPE_CONSTANT         = 1 << 2,
    TOKEN_TYPE_SPACE            = 0,
};

enum PUNCTUATORS
{
    PUNCTUATOR_LBRACE = 0,
    PUNCTUATOR_RBRACE,
    PUNCTUATOR_COMMA,
    PUNCTUATOR_PLUS,
    PUNCTUATOR_MINUS,
    PUNCTUATOR_LAST
};

enum KEYWORDS
{
    KEYWORD_MOV = 0,
    KEYWORD_ADD,
    KEYWORD_SUB,
    KEYWORD_MUL,
    KEYWORD_DIV,
    KEYWORD_CMP,
    KEYWORD_AND,
    KEYWORD_OR,
    KEYWORD_XOR,
    KEYWORD_NOT,
    KEYWORD_SHL,
    KEYWORD_SHR,
    KEYWORD_JMP,
    KEYWORD_JZ,
    KEYWORD_JE,
    KEYWORD_JNE,

    KEYWORD_ACCL,
    KEYWORD_ACCH,
    KEYWORD_ACC,
    KEYWORD_LAST
};

struct token_t
{
    uint16_t type;
    uint16_t token;
};

void next_token();

void parse();

void piss(uint32_t level, const char *fmt, ...);

#endif