#ifndef ASM_H
#define ASM_H

#include <stdint.h>
#include "isa.h"

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
    CHAR_TYPE_LITERAL          = 1 << 3,
    CHAR_TYPE_SPACE            = 0,
};

enum TOKEN_TYPE
{
    TOKEN_TYPE_SPACE            = 0,
    TOKEN_TYPE_PUNCTUATOR,
    TOKEN_TYPE_REG,
    TOKEN_TYPE_OPCODE,
    TOKEN_TYPE_CONSTANT,
    TOKEN_TYPE_LITERAL,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_NAME,
};

enum CONSTANTS 
{
    CONSTANT_DECIMAL        = 0,
    CONSTANT_HEXADECIMAL,
    CONSTANT_BINARY,
    CONSTANT_CHAR,
    CONSTANT_NONE
};

enum PUNCTUATORS
{
    PUNCTUATOR_LBRACE = 0,
    PUNCTUATOR_RBRACE,
    PUNCTUATOR_LPARENTHESIS,
    PUNCTUATOR_RPARENTHESIS,
    PUNCTUATOR_DOT,
    PUNCTUATOR_COMMA,
    PUNCTUATOR_COLLON,
    PUNCTUATOR_PLUS,
    PUNCTUATOR_MINUS,
    PUNCTUATOR_LAST
};

enum KEYWORDS
{
    KEYWORD_STRING = 0,
    KEYWORD_BYTE,
    KEYWORD_WORD,
    KEYWORD_LAST,
};

enum REGS
{
    REG_ACCL        = 0x0,
    REG_ACCH        = 0x1,
    REG_ACCW        = 0x2,
    REG_BASE        = 0x8,
    REG_STT         = 0x9,
    REG_STB         = 0xa,
    REG_CONST       = 0x1e,
    REG_INDIRECT    = 0x1f,  
    REG_LAST
};

#define REG_ACCL_BIT (1 << REG_ACCL)
#define REG_ACCH_BIT (1 << REG_ACCH)
#define REG_ACCW_BIT (1 << REG_ACCW)
#define REG_BASE_BIT (1 << REG_BASE)
#define REG_STT_BIT (1 << REG_STT)
#define REG_STB_BIT (1 << REG_STB)
#define REG_CONST_BIT (1 << REG_CONST)
#define REG_INDIRECT_BIT (1 << REG_INDIRECT)
#define REG_ALL_BIT (REG_ACCL_BIT | REG_ACCH_BIT | REG_ACCW_BIT | REG_BASE_BIT | REG_STT_BIT | REG_STB_BIT)
#define REG_ALL16_BIT (REG_ACCW_BIT | REG_BASE_BIT | REG_STT_BIT | REG_STB_BIT)
#define REG_ALL8_BIT (REG_ACCL_BIT | REG_ACCH_BIT)

struct reg_t
{
    const char *    name;
    uint32_t        value;
};

#define WORD_OPCODE_MASK 0xc0
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
    OPCODE_JE,
    OPCODE_JNZ,
    OPCODE_JNE,

    OPCODE_JA,
    OPCODE_JNBE,
    OPCODE_JNA,
    OPCODE_JBE,
    OPCODE_JAE,
    OPCODE_JNB,
    OPCODE_JNC,
    OPCODE_JNAE,
    OPCODE_JB,
    OPCODE_JC,  
    OPCODE_JG,
    OPCODE_JL,
    OPCODE_CALL,
    OPCODE_RET,
    OPCODE_PUSH,
    OPCODE_POP,
    OPCODE_IN,
    OPCODE_OUT,
    OPCODE_LAST
};

struct operand_t
{
    uint16_t reg;
    uint16_t constant : 1;
    uint16_t indirect : 1;
};

struct opvariant_t
{
    uint16_t opcode;
    uint16_t width; 

    uint32_t operands[2];
};

#define MAX_OPCODE_VARIANTS 0x20
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

#define OUTPUT_BUFFER_SIZE 4096
struct output_buffer_t
{
    struct output_buffer_t *        next;
    uint32_t                        offset;
    uint8_t *                       data;
};

struct token_t
{
    uint32_t        line;
    uint32_t        column;
    uint32_t        type;
    int32_t         token;
    uint32_t        length;
    // uint32_t        last_in_line;
};

void lex_token();

void parse();

void filter_variants(struct opvariant_t *variant_buffer, uint32_t *variant_count, uint32_t operand, uint32_t mask, uint32_t result);

void parse_instruction();

void parse_label();

void parse_declaration();

void emit_byte(uint8_t byte);

void emit_word(uint16_t word);

void emit_opcode(uint16_t opcode);

void patch_code();

struct label_t *create_label_on_list(char *name, uint16_t name_len, uint16_t offset, struct label_t **first, struct label_t **last);

struct label_t *create_label(struct token_t *token, uint16_t offset);

struct label_t *create_patch(struct token_t *token, uint16_t offset);

struct label_t *find_label_from_token(struct token_t *token);

struct label_t *find_label(char *name);

void piss(uint32_t level, const char *fmt, ...);

#endif