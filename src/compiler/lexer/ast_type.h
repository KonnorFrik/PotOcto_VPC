/** @file */
#ifndef __AST_TYPE_H__
#define __AST_TYPE_H__

#include "token.h"

/**
 * @brief One token tree
 * literally this is AST, but with wrong naming
 */
typedef struct ast {
    Token* token;       ///< Token object
    struct ast* left;  ///< Pointer for next same struct with operator/operand
    struct ast* right; ///< Pointer for next same struct with operator/operand
} AST;
#endif /* __AST_TYPE_H__ */
