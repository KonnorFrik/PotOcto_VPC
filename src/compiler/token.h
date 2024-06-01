/**
 * @file All operations for transform line to Token
 */
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdbool.h>
#include "../hardware/basic_types.h" 

/**
 * @brief Struct with one toke
 */
typedef struct {
    int type;    ///< Token type
    char* word;  ///< line with current word
    dword value; ///< value for numbers
} Token;

/**
 * @brief Create Token object and init it
 * @return NULL if can't allocate memory
 * @param[in] type Token type
 * @param[in] word Word for token
 * @param[in] value Value for token
 * @return obj or NULL
 */
Token* create_token(int type, char* word, dword value);

/**
 * @brief Create Token object from given line
 * @warning Exit from program if error occured
 * @param[in] line Line for convert to Token
 * @return token Valid Token object
 */
Token* get_token(char* line);

/**
 * @brief Determine if given word is Keyword
 * @param[in] line Line with word for check
 * @return result 0:NO 1:YES
 */
bool is_line_kw(char* line);

/**
 * @brief Determine if given word starts with any access operator
 * @param[in] line Line with word for check
 * @return result 0:NO 1:YES
 */
bool is_line_access_op(char* line);

/**
 * @brief Determine if given word is number
 * @param[in] line Line with word for check
 * @return result 0:NO 1:YES
 */
bool is_line_number(char* line);

/**
 * @brief Determine number base and convert given word to number
 * @param[in] line Word for convert
 * @return result Converted number 
 */
dword str_to_num(char* line);

#endif /* __TOKEN_H__ */
