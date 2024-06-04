/**
 * @file All operations for manage AST
 */
#ifndef __AST__
#define __AST__

#include "../../hardware/basic_types.h" 
#include "ast_type.h"

#define AST_DEBUG 1

/**
 * @brief Create AST object. Allocate memory and init
 * @note Return NULL if Error occured
 * @return obj AST object or NULL
 */
AST* ast_create();

/**
 * @brief Create AST_ARR object
 * @note Return NULL if Error occured
 * @return obj AST_ARR or NULL
 */
AST_ARR* astarr_create();

/**
 * @brief Parse given line for tokens
 * @note Retrun NULL for every error
 * @param[in] line Current line with words
 * @return ast_node Valid AST object or NULL
 */
AST* ast_tokenize_line(char* line);

/**
 * @brief Part of constructing AST. Append new Token to AST
 * @param[in, out] head Head of AST
 * @param[in]      token Token object for append
 * @param[in]      token_count value for determine where append token
 */
int ast_append_token(AST* head, Token* token, int token_count);

/**
 * @brief Wrap for 'ast_append_token' for manage appending
 * IN: one word, OUT: AST filled with new one token
 * @param[in]      line Current line for create tokens from
 * @param[in, out] ast  AST object for fill it 
 * @param[in]      token_count Number of current word in line
 * @return result Error code or OK
 */
int ast_safe_append_tokens(char* line, AST* ast, int token_count);

/**
 * @brief Destroy AST object
 * @param[in, out] obj Valid AST object
 * @return void
 */
void ast_destroy(AST* obj);

/**
 * @brief Append AST object to ASTs array (AST object)
 * @param[in, out] asts_obj Valid AST object for append in
 * @param[in]      obj      Valid AST object
 * @return status OK or Error code
 */
int astarr_append(AST_ARR* asts_obj, AST* obj);

/**
 * @brief Destroy AST_ARR object
 * @param[in, out] obj Valid AST_ARR object
 * @return void
 */
void astarr_destroy(AST_ARR* obj);

#if AST_DEBUG == 1
void ast_print(AST* node);
#endif

#endif /* __AST__ */
