#ifndef __AST__
#define __AST__

#include "../hardware/basic_types.h" 
#include <stddef.h>

enum {
    KEYWORD = 1,
    MEM_ACCESS_OPERATOR, // 2
    REG_ACCESS_OPERATOR, // 3
    NUMBER,              // 4
};

/**
 * @brief Struct with one toke
 */
typedef struct {
    int type;    ///< Token type
    char* word;  ///< line with current word
    dword value; ///< value for numbers
} Token;

/**
 * @brief One token tree
 * literally this is AST, but with wrong naming
 */
typedef struct ast {
    Token* token;       ///< Token object
    struct ast* left;  ///< Pointer for next same struct with operator/operand
    struct ast* right; ///< Pointer for next same struct with operator/operand
} AST;

/**
 * @brief Array with Nodes (aka ast)
 */
typedef struct {
    AST** array; ///< Array with nodes
    size_t size;  ///< current size
    size_t index; ///< current empty cell index for write
} AST_ARR;

#endif /* __AST__ */
