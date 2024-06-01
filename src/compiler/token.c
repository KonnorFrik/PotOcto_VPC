#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "ast.h"
#include "keywords_array.h"
#include "../common/funcs.h"
#include "../common/error_codes.h"
#include "../str_funcs/str_funcs.h"

#define REG_ACCESS_WORD 'r'
#define MEM_ACCESS_WORD '$'

Token* create_token(int type, char* word, dword value) {
    Token* obj = calloc(1, sizeof(Token));

   if ( obj ) {
        obj->type = type;
        obj->value = value;
        obj->word = calloc(strlen(word) + 1, sizeof(char));

        if ( obj->word ) {
            strcpy(obj->word, word);

        } else {
            free(obj);
            obj = NULL;
        }
    }

    return obj;
}

Token* get_token(char* line) {
    int token_type = INVALID;
    char* word = "";
    dword value = 0;

    if ( is_line_kw(line) ) {
        token_type = KEYWORD;
        word = line;

    } else if ( is_line_access_op(line) ) {
        if ( line[0] == 'r' ) {
            token_type = REG_ACCESS_OPERATOR;

        } else if ( line[0] == '$' ) {
            token_type = MEM_ACCESS_OPERATOR;
        }

    } else if ( is_line_number(line) ) {
        token_type = NUMBER;
        value = str_to_num(line);
    }

    Token* token = create_token(token_type, word, value);

    if ( !token ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "get_token: token is NULL\n");
        exit(MEM_ERROR);
    }

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: Create Token: type: %d value: %u\n", token_type, value);
    #endif

    return token;
}

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

    if (line[0] == REG_ACCESS_WORD || line[0] == MEM_ACCESS_WORD) {
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

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: str_to_num: '%s':type(%d) -> '%d'\n", line, type, result);
    #endif

    return result;
}
