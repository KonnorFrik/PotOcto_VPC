#ifndef __LEXER_H__
#define __LEXER_H__

#include <stddef.h>
#include "ast_type.h"

/**
 * @brief Parse given line for tokens
 * @note Abort program if any error occured
 * @param[in] line Current line with words
 * @param[in] line_count Current line number
 * @return ast_node Valid AST object
 */
AST* tokenize_line(char* line, size_t line_count);

#endif /* __LEXER_H__ */
