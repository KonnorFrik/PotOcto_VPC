#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "../common/funcs.h"
#include "../common/error_codes.h"
#include "../str_funcs/str_funcs.h"

AST* create_node() {
    AST* obj = calloc(1, sizeof(AST));
    return obj;
}

AST* tokenize_line(char* line, size_t line_count) {
    /*Create AST from line*/
    size_t len = strlen(line);
    AST* ast_node = create_node();

    if ( !ast_node ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Tokens_line: ast_node is NULL\n");
        exit(MEM_ERROR);
    }

    char* buffer = calloc(len + 2, sizeof(char));
    
    if ( !buffer ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Tokens_line: buffer line is NULL\n");
        exit(MEM_ERROR);
    }

    strcpy(buffer, line);
    const char* delim = " ";
    char* token = strtok(buffer, delim);

    int err_code = 0;
    //TODO: replace magic numbers with enum
    // if will be replaced appending system - enum don't need
    int token_count = 0; //0-kw 1-op1 2-op2

    while ( token ) {
        err_code = safe_create_append(token, ast_node, token_count);

#if AST_DEBUG == 1
            fprintf(stderr, "\t[AST_DEBUG]: >append code: '%d'\n\n", err_code);
#endif

        if (err_code) {
            continue;
        }

        token_count++;
        token = strtok(NULL, delim);
    }

    free(buffer);

    if (err_code) {
        show_error(err_code);
        fprintf(stderr, "[ERROR] Syntax problem with: #%zu > '%s'\n", line_count, line);
        exit(ERROR);
    }

    return ast_node;
}

int append_to_node(AST* head, Token* token, int token_count) {
    if ( token_count > 2 ) {
        return INVALID_LINE;
    }

    if ( token->type == INVALID ) {
        return INVALID_WORD;
    }

    int result = 0;
    AST* copy = head;

    if ( token_count == 0 ) { //kw
        copy->token = token;
    }

    if ( token_count == 1 ) { //op1
        while ( copy->left != NULL ) {
            copy = copy->left;
        }

        copy->left = create_node();

        if ( copy->left ) {
            copy->left->token = token;

        } else {
            result = MEM_ERROR; // MEM_ERROR
        }
    }

    if ( token_count == 2 ) { //op2
        while ( copy->right != NULL ) {
            copy = copy->right;
        }

        copy->right = create_node();

        if ( copy->right ) {
            copy->right->token = token;

        } else {
            result = MEM_ERROR; // MEM_ERROR
        }
    }

#if AST_DEBUG == 1
        fprintf(stderr, "\t[AST_DEBUG]: Append token: token: %d | count: %d\n", token->type, token_count);
#endif
    
    return result;
}

int safe_create_append(char* line, AST* ast, int token_count) {
    /*append token to AST if it not invalid*/
    int result = OK; // 0

#if AST_DEBUG == 1
        fprintf(stderr, "\t[AST_DEBUG]: word: '%s'\n", line);
#endif

    Token* word_token = get_token(line);

    if (word_token->type == INVALID) {
        result = SYNTAX_ERR;
    }

    result |= append_to_node(ast, word_token, token_count);

    if (word_token->type == MEM_ACCESS_OPERATOR || word_token->type == REG_ACCESS_OPERATOR) {
        result |= safe_create_append(line + 1, ast, token_count);
    }

    return result;
}

int append_tree(AST_ARR* asts_obj, AST* obj) {
    /*Append one ast to array 
     * Potential depricated */
    int status = OK;

    if (asts_obj->index >= asts_obj->size) {
        size_t new_size = (asts_obj->size + (asts_obj->size / 2)) * sizeof(AST*);
        AST** tmp = realloc(asts_obj->array, new_size);

        if ( tmp ) {
            asts_obj->array = tmp;
            asts_obj->size = new_size;

        } else {
            status = MEM_ERROR;
        }
    }

    (asts_obj->array)[(asts_obj->index)++] = obj;

    return status;
}

#if AST_DEBUG == 1
/**
 * @brief Print given AST object to stdout
 * @note For debug use
 */
void print_node(AST* node) {
    /*For debug*/
    if (node->token == 0 || node->left == 0 || node->right == 0) {
        return;
    }

    AST* copy = node;
    printf("\tHead token: %d(%s)\n", copy->token->type, copy->token->type == KEYWORD ? copy->token->word : "");
    printf("\tLeft: ");

    copy = node->left;
    while (copy != NULL) {
        printf("%d(%d) ", copy->token->type, copy->token->value);
        copy = copy->left;
    }

    printf("\n");
    copy = node->right;
    printf("\tRight: ");

    while (copy != NULL) {
        printf("%d(%d) ", copy->token->type, copy->token->value);
        copy = copy->right;
    }

    printf("\n\n");
}
#endif

