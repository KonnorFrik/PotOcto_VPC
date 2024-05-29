#ifndef __AST__
#define __AST__

#include "../hardware/config.h" 
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

// TODO: Change name 'Node' to 'AST'. 'AST' to smth like 'AST_ARRAY'
/**
 * @brief One token tree
 * literally this is AST, but with wrong naming
 */
typedef struct node {
    Token* token;       ///< Token object
    struct node* left;  ///< Pointer for next same struct with operator/operand
    struct node* right; ///< Pointer for next same struct with operator/operand
} Node;

/**
 * @brief Array with Nodes (aka ast)
 */
typedef struct {
    Node** array; ///< Array with nodes
    size_t size;  ///< current size
    size_t index; ///< current empty cell index for write
} AST;

#endif /* __AST__ */
