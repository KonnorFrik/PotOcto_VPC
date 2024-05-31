#ifndef __STR_FUNCS_H__
#define __STR_FUNCS_H__

#include <stddef.h>
#include <stdbool.h>

typedef enum str_type {
    BIN = 2,
    DEC = 10,
    HEX = 16,
    INVALID = -1,
} str_type;

/**
 * @brief Check is given string starts with given format
 * @param[in] str String for check 
 * @param[in] format Format string for check with it
 * @return result false, true
 */
bool startswith(const char* str, const char* format);

/**
 * @brief Check is given string contain decimal numbers
 * @param[in] str String for check
 * @return 0:NO 1:YES
 */
bool is_str_dec(const char* str);

/**
 * @brief Check is given string contain binary numbers
 * @param[in] str String for check
 * @return 0:NO 1:YES
 */
bool is_str_bin(const char* str);

/**
 * @brief Check is given string contain hexadecimal numbers
 * @param[in] str String for check
 * @return 0:NO 1:YES
 */
bool is_str_hex(const char* str);

/**
 * @brief Check is given char hexadecimal 
 * @param[in] s Letter for check
 * @return result 0:NO 1:YES
 */
bool is_hex(char s);

/**
 * @brief Determine type of given string
 * @param[in] str String for get it type
 * @return result Value from 'enum str_type'
 */
str_type string_type(const char* str);

/**
 * @brief Replace first matched with 'from' letter in given string
 * @param[in] line String for replace it letters
 * @param[in] from Letter for replace this
 * @param[in] to   Letter for replace with this
 * @return status 0:NotReplaced 1:Replaced
 */
bool replace_f(char* line, char from, char to);

#endif /* __STR_FUNCS_H__ */
