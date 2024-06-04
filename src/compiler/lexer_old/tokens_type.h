#ifndef __TOKENS_TYPE__
#define __TOKENS_TYPE__ 

#include "../../hardware/basic_types.h" 

enum token_type {
    KEYWORD = 1,
    MEM_ACCESS_OPERATOR, // 2
    REG_ACCESS_OPERATOR, // 3
    NUMBER,              // 4
    COMMENT,
};

/**
 * @brief Struct with one toke
 */
typedef struct {
    int type;    ///< Token type
    char* word;  ///< line with current word
    dword value; ///< value for numbers
} Token;


#endif /* __TOKENS_TYPE__ */
