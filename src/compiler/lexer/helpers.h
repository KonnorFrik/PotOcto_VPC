/** @file */
#ifndef __LEXER_HELPERS_H__
#define __LEXER_HELPERS_H__

#include <stdbool.h>
#include "../../hardware/basic_types.h" 

#define AO_WORD_REGISTER 'r'
#define AO_WORD_MEMORY '$'
#define KW_COMMENT ';'

/**
 * @brief All possible number types in string
 * @note value prepared for pass in 'strtol'
 */
typedef enum str_type {
    BIN = 2,
    DEC = 10,
    HEX = 16,
    INVALID = -1,
} str_type;

/**
 * @brief Determine if given word is Keyword
 * @param[in] line Line with word for check
 * @return result true or false
 */
bool is_line_kw(char* line);

/**
 * @brief Determine if given word starts with any access operator
 * @param[in] line Line with word for check
 * @return result true or false
 */
bool is_line_access_op(char* line);

/**
 * @brief Determine if given word is number
 * @param[in] line Line with word for check
 * @return result true or false
 */
bool is_line_number(char* line);

/**
 * @brief Check is line full comment
 * @param[in] line Valid line
 * @return status true or false
bool is_line_comment(char* line);
 */

/**
 * @brief Check if line starts with given line
 * @param[in] str String for check it prefix
 * @param[in] format String with prefix for compare with it
 * @return result true or false
 */
bool startswith(const char* str, const char* format);

/**
 * @brief Determine if string with number is binary
 * @param[in] str String with nubmer
 * @return result true or false
 */
bool is_str_bin(const char* str);

/**
 * @brief Determine if string with number is decimal
 * @param[in] str String with nubmer
 * @return result true or false
 */
bool is_str_dec(const char* str);

/**
 * @brief Determine if string with number is hexadecimal
 * @param[in] str String with nubmer
 * @return result true or false
 */
bool is_str_hex(const char* str);

/**
 * @brief Determine if one letter in hex group
 * @param[in] s Char for check
 * @return result true or false
 */
bool is_char_hex(char s);

/**
 * @brief Determine number base and convert given line to number
 * @param[in] line Word for convert
 * @return result Converted number 
 */
dword str_to_num(char* line);

/**
 * @brief Determine type of line with number
 * @param[in] str String with number
 * @return type From str_type
 */
str_type string_type(const char* str);

/**
 * @brief Check if one word is comment symbol or starts with
 * @param[in] line String with one word
 * @return result true or false
 */
bool is_line_comment(char* line);

#endif /* __LEXER_HELPERS_H__ */
