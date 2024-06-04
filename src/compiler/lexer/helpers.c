#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "helpers.h"
#include "../keywords_array.h"
// #include "lexer.h"

#define LXR_DEBUG 0

bool is_line_kw(char* line) {
    bool result = false;
    int ind = 0;

    while ( !result && Keywords[ind] != NULL ) {
        if ( strcmp(line, Keywords[ind]) == 0 ) {
            result = true;
        }

        ind++;
    }

    return result;
}

bool is_line_access_op(char* line) {
    bool result = false;

    if (line[0] == AO_WORD_REGISTER || line[0] == AO_WORD_MEMORY) {
        result = true;
    }

    return result;
}

bool is_line_number(char* line) {
    bool result = false;

    if ( string_type((const char*)line) != INVALID ) {
        result = true;
    }

    return result;
}

dword str_to_num(char* line) {
    dword result = 0;
    str_type type = string_type(line);
    int base = 0;

    if ( type == BIN ) {
        base = 2;
        line += 2;

    } else if ( type == HEX ) {
        base = 16;
        line += 2;
    }

    result = (dword)strtol(line, NULL, base);

#if LXR_DEBUG == 1
        fprintf(stderr, "\t[LXR_DEBUG]: str_to_num: '%s':type(%d) -> '%d'\n", line, type, result);
#endif

    return result;
}

bool startswith(const char* str, const char* format) {
  bool result = true;

  int ind = 0;
  int run_flag = 1;

  while (str[ind] != '\0' && format[ind] != '\0' && run_flag) {
    if (str[ind] != format[ind]) {
      result = false;
      run_flag = 0;
    }

    ind++;
  }

  return result;
}

bool is_str_bin(const char* str) {
    bool result = true;
    int len = strlen(str);

    if (len < 3 || !startswith(str, "0b")) {
        result = false;

    } else {
        str += 2; // skip '0b'
        len -= 2;
    }

    for (int i = 0; result && i < len; ++i) {
        if (str[i] != '0' && str[i] != '1') {
            result = false;
        }
    }

#if STR_DEBUG == 1
        fprintf(stderr, "\t[STR_DEBUG]: is_str_bin: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[STR_DEBUG]: is_str_bin: result; %d\n", result);
#endif

    return result;
}

bool is_str_dec(const char* str) {
    bool result = true;
    int len = strlen(str);

    if (len < 1) {
        result = false;
    }

    for (int i = 0; result && i < len; ++i) {
        if (!isdigit(str[i])) {
            result = false;
        }
    }

#if STR_DEBUG == 1
        fprintf(stderr, "\t[STR_DEBUG]: is_str_dec: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[STR_DEBUG]: is_str_dec: result; %d\n", result);
#endif
    
    return result;
}

bool is_str_hex(const char* str) {
    bool result = true;
    int len = strlen(str);

    if (len < 3 || !startswith(str, "0x")) {
        result = false;

    } else {
        str += 2; // skip '0x'
        len -= 2;
    }

    for (int i = 0; result && i < len; ++i) {
        if (!is_char_hex(str[i])) {
            result = false;
        }
    }

#if STR_DEBUG == 1
        fprintf(stderr, "\t[STR_DEBUG]: is_str_hex: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[STR_DEBUG]: is_str_hex: result; %d\n\n", result);
#endif

    return result;
}

bool is_char_hex(char s) {
    int is_num = isdigit(s);
    int is_hex_small = (s >= 'a' && s <= 'f');
    int is_hex_big = (s >= 'A' && s <= 'F');

    return is_num || (is_hex_small || is_hex_big);
}

str_type string_type(const char* str) {
    int result = INVALID;

    if (is_str_bin(str)) {
        result = BIN;
    }

    if (result == INVALID && is_str_dec(str)) {
        result = DEC;
    }

    if (result == INVALID && is_str_hex(str)) {
        result = HEX;
    }

    return result;
}

bool is_line_comment(char* line) {
    return line && (line[0] == 0 || line[0] == KW_COMMENT);
}
