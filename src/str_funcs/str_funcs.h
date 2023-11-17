#ifndef __STR_FUNCS_H__
#define __STR_FUNCS_H__

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "../config.h" 

enum str_type {
    BIN = 2,
    DEC = 10,
    HEX = 16,
    INVALID = -1,
};

int startswith(const char* str, const char* format);
int is_str_dec(const char* str);
int is_str_bin(const char* str);
int is_str_hex(const char* str);
int is_hex(char s);
int string_type(const char* str);

#endif
