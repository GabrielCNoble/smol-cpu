#ifndef ASM_H
#define ASM_H

#include <stdint.h>

enum PISS_LEVEL
{
    PISS_ERROR = 0,
    PISS_WARNING,
    PISS_MESSAGE,
};

enum CHAR_TYPE
{
    CHAR_TYPE_PUNCTUATOR       = 1,
    CHAR_TYPE_KEYWORD          = 1 << 1,
    CHAR_TYPE_CONSTANT         = 1 << 2,
    CHAR_TYPE_SPACE            = 0,
};

enum TOKEN_TYPE
{
    TOKEN_TYPE_SPACE            = 0,
    TOKEN_TYPE_PUNCTUATOR,
    TOKEN_TYPE_REG,
    TOKEN_TYPE_OPCODE,
    TOKEN_TYPE_CONSTANT,
};

enum CONSTANTS 
{
    CONSTANT_DECIMAL        = 0,
    CONSTANT_HEXADECIMAL,
    CONSTANT_BINARY,
    CONSTANT_NONE
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

// enum KEYWORDS
// {
//     KEYWORD_MOV = 0,
//     KEYWORD_ADD,
//     KEYWORD_SUB,
//     KEYWORD_MUL,
//     KEYWORD_DIV,
//     KEYWORD_CMP,
//     KEYWORD_AND,
//     KEYWORD_OR,
//     KEYWORD_XOR,
//     KEYWORD_NOT,
//     KEYWORD_SHL,
//     KEYWORD_SHR,
//     KEYWORD_JMP,
//     KEYWORD_JZ,
//     KEYWORD_JNZ,
//     KEYWORD_JE,
//     KEYWORD_JNE,

//     KEYWORD_CALL,

//     KEYWORD_ACCL,
//     KEYWORD_ACCH,
//     KEYWORD_ACC,
//     KEYWORD_BASE, 
//     KEYWORD_STT,
//     KEYWORD_STB,
//     KEYWORD_LAST
// };

enum REGS
{
    REG_ACCL   = 0x0,
    REG_ACCH   = 0x1,
    REG_ACCW   = 0x2,
    REG_BASE   = 0x8,
    REG_STT    = 0x9,
    REG_STB    = 0xa,
    REG_LAST
};

struct reg_t
{
    const char *    name;
    uint32_t        value;
};

enum OPCODES
{
    OPCODE_MOV,
    OPCODE_ADD,
    OPCODE_SUB,
    OPCODE_MUL,
    OPCODE_DIV,
    OPCODE_CMP,
    OPCODE_AND,
    OPCODE_OR,
    OPCODE_XOR,
    OPCODE_NOT,
    OPCODE_SHL,
    OPCODE_SHR,
    OPCODE_INC,
    OPCODE_DEC,
    OPCODE_JMP,
    OPCODE_JZ,
    OPCODE_JNZ,
    OPCODE_CALL,
    OPCODE_RET,
    OPCODE_LAST
};

struct opvariant_t
{
    uint8_t opcode[4];
    uint16_t src_reg;
    uint16_t dst_reg;
};

struct opcode_t
{
    const char *            name;
    uint32_t                variant_count;
    struct opvariant_t *    variants;
};

struct label_t
{
    struct label_t *    next;
    char *              name;
    uint16_t            offset;
};

// enum KEYWORD_TYPES
// {
//     KEYWORD_TYPE_INSTRUCTION,
//     KEYWORD_TYPE_REGISTER
// };

// struct keyword_t
// {
//     char *      text;
//     uint16_t    type;
// };

struct token_t
{
    uint16_t        type;
    uint16_t        token;
    uint32_t        line;
    uint32_t        column;
};

void next_token();

void parse();

void parse_instruction();

void piss(uint32_t level, const char *fmt, ...);

#endif