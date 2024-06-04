#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../common/error_codes.h"
#include "lexer.h"
#include "helpers.h"

#define LXR_DEBUG 0

AST* lexer_tokenize_line(char* line) {
    if ( !line ) {
        return NULL;
    }

    int status = OK; // local status code
    size_t len = strlen(line);
    char* buffer = NULL;
    AST* ast_node = ast_create();

    if ( !ast_node ) {
        status = MEM_ERROR;
    }

    if ( status == OK ) {
        buffer = calloc(len + 1, sizeof(char));

        if ( !buffer ) {
            status = MEM_ERROR;
        }
    }
    
    char* token = NULL;
    const char* delim = " ";

    if ( !status ) {
        strcpy(buffer, line);
        token = strtok(buffer, delim);
    }

    while ( (status == OK) && token ) {
        Token* word_token = token_get(token);

        if ( !word_token || word_token->type == INVALID ) {
            status = SYNTAX_ERR;
            continue;
        }

        token = strtok(NULL, delim);
    }

    if ( buffer ) {
        free(buffer);
    }

    if ( status != OK ) {
        if ( ast_node ) {
            free(ast_node);
        }

        ast_node = NULL;
    }

    return ast_node;
}

Token* token_create(token_type type, char* word, dword value) {
    Token* obj = calloc(1, sizeof(Token));

   if ( obj ) {
        obj->type = type;
        obj->value = value;
        obj->line = calloc(strlen(word) + 1, sizeof(char));

        if ( obj->line ) {
            strcpy(obj->line, word);

        } else {
            token_destroy(obj);
            obj = NULL;
        }
    }

    return obj;
}

// TODO: write tests for this
Token* token_get(char* line) {
    int token_type = INVALID;
    char* word = "";
    dword value = 0;

    if ( is_line_kw(line) ) {
        token_type = KEYWORD;
        word = line;

    } else if ( is_line_access_op(line) ) {
        if ( line[0] == AO_WORD_REGISTER ) {
            token_type = AO_REGISTER;

        } else if ( line[0] == AO_WORD_MEMORY ) {
            token_type = AO_MEMORY;
        }

        word = line;

    } else if ( is_line_number(line) ) {
        token_type = NUMBER;
        value = str_to_num(line);
        word = line;

    } else if ( is_line_comment(line) ) {
        token_type = COMMENT;
        word = line;
    }

    Token* token = token_create(token_type, word, value);

    #if LXR_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: Created Token: type: %d value: %u\n", token_type, value);
    #endif

    return token;
}

void token_destroy(Token* obj) {
    if ( !obj ) {
        return;
    }

    if ( obj->line ) {
        free(obj->line);
    }

    free(obj);
}

AST* ast_create() {
    AST* obj = calloc(1, sizeof(AST));

    // if ( obj ) {
    // }

    return obj;
}

void ast_destroy(AST* obj) {
    if ( !obj ) {
        return;
    }

    AST* prev = obj;

    while ( obj->next != NULL ) {
        token_destroy(obj->token);
        prev = obj;
        obj = obj->next;
        free(prev);
    }

    token_destroy(obj->token);
    free(obj);
}

int ast_append_token(AST* head, Token* token) {
    if ( !head || !token ) {
        return MEM_ERROR;
    }
    
    int status = OK;

    while ( head->next != NULL ) {
        head = head->next;
    }

    head->next = ast_create();
    
    if ( head->next ) {
        head->next->token = token;

    } else {
        status = MEM_ERROR;
    }

    return status;
}

