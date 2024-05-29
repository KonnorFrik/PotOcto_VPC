#ifndef __STR_FUNCS_H__
#define __STR_FUNCS_H__

#include <stddef.h>

enum str_type {
    BIN = 2,
    DEC = 10,
    HEX = 16,
    INVALID = -1,
};

/**
 * @brief Check is given string starts with given format
 * @param[in] str String for check 
 * @param[in] format Format string for check with it
 * @return result 0:NO 1:YES 
 */
int startswith(const char* str, const char* format);

/**
 * @brief Check is given string contain decimal numbers
 * @param[in] str String for check
 * @return 0:NO 1:YES
 */
int is_str_dec(const char* str);

/**
 * @brief Check is given string contain binary numbers
 * @param[in] str String for check
 * @return 0:NO 1:YES
 */
int is_str_bin(const char* str);

/**
 * @brief Check is given string contain hexadecimal numbers
 * @param[in] str String for check
 * @return 0:NO 1:YES
 */
int is_str_hex(const char* str);

/**
 * @brief Check is given char hexadecimal 
 * @param[in] s Letter for check
 * @return result 0:NO 1:YES
 */
int is_hex(char s);

/**
 * @brief Determine type of given string
 * @param[in] str String for get it type
 * @return result Value from 'enum str_type'
 */
int string_type(const char* str);

/**
 * @brief Replace letters in given string
 * @param[in] line String for replace it letters
 * @param[in] from Letter for replace this
 * @param[in] to   Letter for replace to this
 * @return new_len Length of string before replaced letter
 */
size_t replace(char* line, char from, char to);

#endif /* __STR_FUNCS_H__ */
