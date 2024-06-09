#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../common/error_codes.h"
#include "lexer.h"
#include "helpers.h"

#define LXR_DEBUG 1

void token_print(Token* obj);
void ast_print(AST* obj);

AST* lexer_tokenize_line(char* line, Options* opt) {
    if ( !line || !opt ) {
        return NULL;
    }

    int status = OK; // local status code
    size_t len = strlen(line);
    char* buffer = NULL;
    AST* ast_node = NULL;

    // fast process commented line
    // TODO: try discard commented line
    // need return error code with info like 'ret_code = its_comment_its_okay_to_return_null'
    if ( line[0] == WORD_COMMENT ) {
        Token* tok = token_get(line);

        if ( !tok ) {
            status = MEM_ERROR;

        } else {
            status = ast_append_token(&ast_node, tok);
        }

        if ( status != OK && ast_node ) {
            ast_destroy(ast_node);
            status = MEM_ERROR;
        }

        return ast_node;
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

        if ( !word_token ) {
            status = MEM_ERROR;
            continue;
        }

        // if ( opt->verbose ) {
        //     fprintf(stderr, "[LEXER]: Created Token: type: %d value: %u\n", word_token->type, word_token->value);
        // }

        status = ast_append_token(&ast_node, word_token);
        token = strtok(NULL, delim);
    }

    if ( buffer ) {
        free(buffer);
    }

    if ( status != OK ) {
        if ( ast_node ) {
            ast_destroy(ast_node);
        }

        ast_node = NULL;
    }

    if ( opt->lexer_show_ast ) {
        ast_print(ast_node);
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

Token* token_get(char* line) {
    token_type token_type = UNKNOWN;
    // char* word = "";
    dword value = 0;

    if ( is_line_kw(line) ) {
        token_type = KEYWORD;
        // word = line;

    } else if ( is_line_access_op(line) ) {
        if ( line[0] == AO_WORD_REGISTER ) {
            token_type = AO_REGISTER;

        } else if ( line[0] == AO_WORD_MEMORY ) {
            token_type = AO_MEMORY;
        }

        value = str_to_num(line + 1);

    } else if ( is_line_number(line) ) {
        token_type = NUMBER;
        value = str_to_num(line);
        // word = line;

    } else if ( is_line_comment(line) ) {
        token_type = COMMENT;

    } else if ( is_line_label(line) ) {
        token_type = LABEL_START;
    }

    Token* token = token_create(token_type, line, value);

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

int ast_append_token(AST** head, Token* token) {
    if ( !head || !token ) {
        return MEM_ERROR;
    }

    int status = OK;

    if ( !(*head) ) {
        (*head) = ast_create();

        if ( (*head) ) {
            (*head)->token = token;

        } else {
            status = MEM_ERROR;
        }

        return status;
    }

    AST* copy = *head;
    
    while ( copy->next != NULL ) {
        copy = copy->next;
    }

    copy->next = ast_create();
    
    if ( copy->next ) {
        copy->next->token = token;

    } else {
        status = MEM_ERROR;
    }

    return status;
}

void token_print(Token* obj) {
    fprintf(stderr, "[LEXER]: Token %p ", (void*)obj);
    fprintf(stderr, "\ttype:%d ", obj->type);
    fprintf(stderr, "line:'%s' ", obj->line);
    fprintf(stderr, "val:%u", obj->value);
    fprintf(stderr, "\n");
}

void ast_print(AST* obj) {
    fprintf(stderr, "\n\t[LEXER]: AST %p\n", (void*)obj);

    while ( obj ) {
        token_print(obj->token);
        obj = obj->next;
    }

    fprintf(stderr, "\n");
}
