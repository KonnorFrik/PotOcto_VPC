// TODO: lexer must take line, try tokenize it, return tokens tree or NULL
// NULL if error or comment or no tokens in line

#ifndef __LEXER_H__
#define __LEXER_H__

#include <stddef.h>
#include "ast_type.h"

/**
 * @brief Parse given line for tokens
 * @note Abort program if any error occured
 * @param[in] line Current line with words
 * @return ast_node Valid AST object
 */
AST* tokenize_line(char* line);

#endif /* __LEXER_H__ */
