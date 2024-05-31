#include <string.h>
#include <ctype.h>

#include "str_funcs.h"

#define STR_DEBUG 0

#if STR_DEBUG == 1
    #include <stdio.h>
#endif

// TODO: change return type for 1:Changed 0:NOT changed
// change name to 'replace_f'
// make different versions of this function for 
// replace all
// replace first from end
int replace_f(char* line, char from, char to) {
    size_t ind = 0;
    int loop = 1;

    while (loop && line[ind] != 0) {
        if (line[ind] == from) {
            line[ind] = to;
            loop = 0;
            continue;
        }

        ind++;
    }

    return !loop;
}

int startswith(const char* str, const char* format) {
  int result = 1;

  int ind = 0;
  int run_flag = 1;

  while (str[ind] != '\0' && format[ind] != '\0' && run_flag) {
    if (str[ind] != format[ind]) {
      result = 0;
      run_flag = 0;
    }

    ind++;
  }

  return result;
}

int is_str_dec(const char* str) {
    int result = 1;
    int len = strlen(str);

    if (len < 1) {
        result = 0;
    }

    for (int i = 0; result && i < len; ++i) {
        if (!isdigit(str[i])) {
            result = 0;
        }
    }

#if STR_DEBUG == 1
        fprintf(stderr, "\t[STR_DEBUG]: is_str_dec: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[STR_DEBUG]: is_str_dec: result; %d\n", result);
#endif
    
    return result;
}

int is_str_bin(const char* str) {
    int result = 1;
    int len = strlen(str);

    if (len < 3 || !startswith(str, "0b")) {
        result = 0;

    } else {
        str += 2; // skip '0b'
        len -= 2;
    }

    for (int i = 0; result && i < len; ++i) {
        if (str[i] != '0' && str[i] != '1') {
            result = 0;
        }
    }

#if STR_DEBUG == 1
        fprintf(stderr, "\t[STR_DEBUG]: is_str_bin: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[STR_DEBUG]: is_str_bin: result; %d\n", result);
#endif

    return result;
}

int is_str_hex(const char* str) {
    int result = 1;
    int len = strlen(str);

    if (len < 3 || !startswith(str, "0x")) {
        result = 0;

    } else {
        str += 2; // skip '0x'
        len -= 2;
    }

    for (int i = 0; result && i < len; ++i) {
        if (!is_hex(str[i])) {
            result = 0;
        }
    }

#if STR_DEBUG == 1
        fprintf(stderr, "\t[STR_DEBUG]: is_str_hex: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[STR_DEBUG]: is_str_hex: result; %d\n\n", result);
#endif

    return result;
}

int is_hex(char s) {
    int is_num = isdigit(s);
    int is_hex_small = (s >= 'a' && s <= 'f');
    int is_hex_big = (s >= 'A' && s <= 'F');

    return is_num || (is_hex_small || is_hex_big);
}

int string_type(const char* str) {
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

