/**
 * @file All operations for manage AST
 */
#ifndef __AST__
#define __AST__

#include <stddef.h>
#include "token.h"
#include "../hardware/basic_types.h" 

#define AST_DEBUG 0

enum {
    KEYWORD = 1,
    MEM_ACCESS_OPERATOR, // 2
    REG_ACCESS_OPERATOR, // 3
    NUMBER,              // 4
};

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
 * @brief Array with one ast
 */
typedef struct {
    AST** array; ///< Array with nodes
    size_t size;  ///< current size
    size_t index; ///< current empty cell index for write
} AST_ARR;

/**
 * @brief Create AST object. Allocate memory and init
 * @note Return NULL if Error occured
 * @return obj AST object or NULL
 */
AST* create_node();

/**
 * @brief Parse given line for tokens
 * @note Abort program if any error occured
 * @param[in] line Current line with words
 * @param[in] line_count Current line number
 * @return ast_node Valid AST object
 */
AST* tokenize_line(char* line, size_t line_count);

/**
 * @brief Part of constructing AST. Append new Token to AST
 * @param[in, out] head Head of AST
 * @param[in]      token Token object for append
 * @param[in]      token_count value for determine where append token
 */
int append_to_node(AST* head, Token* token, int token_count);

/**
 * @brief Wrap for 'append_to_node' for manage appending
 * IN: one word, OUT: AST filled with new one token
 * @param[in]      line Current line for create tokens from
 * @param[in, out] ast  AST object for fill it 
 * @param[in]      token_count Number of current word in line
 * @return result Error code or OK
 */
int safe_create_append(char* line, AST* ast, int token_count);

/**
 * @brief Append AST object to ASTs array (AST object)
 * @param[in, out] asts_obj Valid AST object for append in
 * @param[in]      obj      Valid AST object
 * @return status OK or Error code
 */
int append_tree(AST_ARR* asts_obj, AST* obj);

#if AST_DEBUG == 1
void print_node(AST* node);
#endif

#endif /* __AST__ */
