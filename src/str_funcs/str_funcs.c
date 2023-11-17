#include "str_funcs.h"

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

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: is_str_dec: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[DEBUG]: is_str_dec: result; %d\n", result);
    }
    
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

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: is_str_bin: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[DEBUG]: is_str_bin: result; %d\n", result);
    }

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

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: is_str_hex: '%s' len: %d\n", str, len);
        fprintf(stderr, "\t[DEBUG]: is_str_hex: result; %d\n", result);
    }

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

    if (result == INVALID && is_str_bin(str)) {
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

