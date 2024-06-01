#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
// #include "../../common/funcs.h"
#include "../../common/error_codes.h"
#include "../../str_funcs/str_funcs.h"

AST* create_node() {
    AST* obj = calloc(1, sizeof(AST));
    return obj;
}

AST* tokenize_line(char* line) {
    if ( !line ) {
        return NULL;
    }

    int status = OK; // local status code
    size_t len = strlen(line);
    AST* ast_node = create_node();

    if ( !ast_node ) {
        status = MEM_ERROR;
    }

    char* buffer = calloc(len + 2, sizeof(char));
    
    if ( !buffer ) {
        status = MEM_ERROR;
    }

    char* token = NULL;
    const char* delim = " ";

    if ( !status ) {
        strcpy(buffer, line);
        token = strtok(buffer, delim);
    }

    //TODO: replace magic numbers with enum
    // if will be replaced appending system - enum don't need
    int token_count = 0; //0-kw 1-op1 2-op2

    while ( (status == OK) && token ) {
        int err_code = safe_create_append(token, ast_node, token_count);

#if AST_DEBUG == 1
            fprintf(stderr, "\t[AST_DEBUG]: >append code: '%d'\n\n", err_code);
#endif

        if ( err_code ) {
            status = ERROR;
            continue;
        }

        token_count++;
        token = strtok(NULL, delim);
    }

    free(buffer);

    if ( status != OK ) {
        free(ast_node);
        ast_node = NULL;
    }

    return ast_node;
}

int append_to_node(AST* head, Token* token, int token_count) {
    if ( !head || !token ) {
        return MEM_ERROR;
    }

    if ( token_count > 2 ) {
        return INVALID_LINE;
    }

    if ( token->type == INVALID ) {
        return INVALID_WORD;
    }

    int status = 0;
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
            status = MEM_ERROR; // MEM_ERROR
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
            status = MEM_ERROR;
        }
    }

#if AST_DEBUG == 1
        fprintf(stderr, "\t[AST_DEBUG]: Append token: token: %d | count: %d\n", token->type, token_count);
#endif
    
    return status;
}

int safe_create_append(char* line, AST* ast, int token_count) {
    /*append token to AST if it not invalid*/
    int status = OK;

#if AST_DEBUG == 1
        fprintf(stderr, "\t[AST_DEBUG]: word: '%s'\n", line);
#endif

    Token* word_token = get_token(line);

    if ( !word_token && (word_token->type == INVALID) ) {
        status = ERROR;
    }

    if ( status == OK ) status = append_to_node(ast, word_token, token_count);

    if ( (status == OK) && (word_token->type == MEM_ACCESS_OPERATOR || word_token->type == REG_ACCESS_OPERATOR) ) {
        status = safe_create_append(line + 1, ast, token_count);
    }

    return status;
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

    if ( status == OK ) (asts_obj->array)[(asts_obj->index)++] = obj;

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

