/** @file */
#ifndef __LEXER_H__
#define __LEXER_H__ 

#include "../../hardware/basic_types.h" 

/**
 * @brief Declare all possible token types 
 */
typedef enum token_type {
    UNKNOWN = 0,
    KEYWORD,
    AO_REGISTER, ///< AO mean 'access operator'
    AO_MEMORY,
    NUMBER,      ///< any type of number (bin, dec, hex)
    COMMENT,
} token_type;

typedef struct token {
    token_type type;
    char* line;  ///< copy of word which converted into token
    dword value; ///< for tokens which converted into value (numbers, register, mem)
} Token;

/**
 * @brief AST like a linked list
 */
typedef struct ast {
    Token* token;
    struct ast* next; ///< next struct or NULL if end
} AST;

/**
 * @brief Convert givin line to one token tree
 * @param[in] line Line with pasm code for tokenize
 * return obj Valid AST object 
 */
AST* lexer_tokenize_line(char* line);

/**
 * @brief Create Token object and init it
 * @param[in] type Token type
 * @param[in] word Word for token
 * @param[in] value Value for token
 * @return obj or NULL
 */
Token* token_create(token_type type, char* word, dword value);

/**
 * @brief Create Token object from given line
 * @param[in] line Line with one word for convert to Token
 * @return token Valid Token object
 */
Token* token_get(char* line);

/**
 * @brief Destroy Token object
 * @param[in, out] obj Token object
 * @return void
 */
void token_destroy(Token* obj);

/**
 * @brief Create AST object. Allocate memory and init
 * @note Return NULL if Error occured
 * @return obj AST object or NULL
 */
AST* ast_create();

/**
 * @brief Destroy AST object
 * @param[in, out] obj AST object
 * @return void
 */
void ast_destroy(AST* obj);

/**
 * @brief Add new token at the end of AST
 * @param[in, out] head Head of AST
 * @param[in]      token Token obect for appending
 * @return status From 'enum error_codes'
 */
int ast_append_token(AST* head, Token* token);

#endif /* __LEXER_H__ */
