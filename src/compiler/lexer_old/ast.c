#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
// #include "../../common/funcs.h"
#include "../../common/error_codes.h"
#include "../../str_funcs/str_funcs.h"
#include "tokens_type.h"

AST* ast_create() {
    AST* obj = calloc(1, sizeof(AST));
    return obj;
}

AST_ARR* astarr_create() {
    AST_ARR* obj = calloc(1, sizeof(AST_ARR));

    if ( obj ) {
        obj->size = 1;
        obj->array = calloc(obj->size, sizeof(AST*)); 

        if ( !obj->array ) {
            free(obj);
            obj = NULL;
        }
    }

    return obj;
}

AST* ast_tokenize_line(char* line) {
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
        buffer = calloc(len + 2, sizeof(char));

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

    //TODO: replace magic numbers with enum
    // if will be replaced appending system - enum don't need
    int token_count = 0; //0-kw 1-op1 2-op2

    while ( (status == OK) && token ) {
        int err_code = ast_safe_append_tokens(token, ast_node, token_count);

#if AST_DEBUG == 1
            fprintf(stderr, "\t[AST_DEBUG]: >append code: '%d'\n\n", err_code);
#endif
        
        if ( ast_node->token->type == COMMENT ) {
            break;
        }

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

int ast_append_token(AST* head, Token* token, int token_count) {
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

        copy->left = ast_create();

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

        copy->right = ast_create();

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

int ast_safe_append_tokens(char* line, AST* ast, int token_count) {
    /*append token to AST if it not invalid*/
    int status = OK;

#if AST_DEBUG == 1
        fprintf(stderr, "\t[AST_DEBUG]: word: '%s'\n", line);
#endif

    Token* word_token = get_token(line);

    if ( !word_token && (word_token->type == INVALID) ) {
        status = ERROR;
    }

    if ( status == OK ) status = ast_append_token(ast, word_token, token_count);

    if ( (status == OK) && (word_token->type == MEM_ACCESS_OPERATOR || word_token->type == REG_ACCESS_OPERATOR) ) {
        status = ast_safe_append_tokens(line + 1, ast, token_count);
    }

    return status;
}

int astarr_append(AST_ARR* asts_obj, AST* obj) {
    /*Append one ast to array 
     * Potential depricated */
    int status = OK;

    if (asts_obj->index >= asts_obj->size) {

        size_t new_size = (asts_obj->size + (asts_obj->size / 2)) + 1;

#if AST_DEBUG == 1
        fprintf(stderr, "[AST_DEBUG]: realloc AST_ARR->array:\n");
        fprintf(stderr, "[AST_DEBUG]: \tindex:     %zu\n", asts_obj->index);
        fprintf(stderr, "[AST_DEBUG]: \tprev size: %zu bytes\n", asts_obj->size);
        fprintf(stderr, "[AST_DEBUG]: \tnew  size: %zu cells\n", new_size);
        fprintf(stderr, "[AST_DEBUG]: \tnew  size: %zu bytes\n", new_size * sizeof(AST*));
#endif

        AST** tmp = realloc(asts_obj->array, new_size * sizeof(AST*));

#if AST_DEBUG == 1
        fprintf(stderr, "[AST_DEBUG]: \tnew addr: %p\n", (void*)tmp);
#endif

        if ( tmp ) {
            asts_obj->array = tmp;
            asts_obj->size = new_size;

        } else {
            status = MEM_ERROR;
        }
    }

    if ( status == OK ) {
        (asts_obj->array)[(asts_obj->index)++] = obj;
    }

    return status;
}

void ast_destroy(AST* obj) {
    if ( !obj ) {
        return;
    }

    if ( obj->token ) {
        token_destroy(obj->token);
    }

    free(obj);
}

void astarr_destroy(AST_ARR* obj) {
    if ( !obj ) {
        return;
    }

    for (size_t ind = 0; ind < obj->index; ++ind) {
        ast_destroy(obj->array[ind]);
    }

    free(obj);
}

#if AST_DEBUG == 1
/**
 * @brief Print given AST object to stdout
 * @note For debug use
 */
void ast_print(AST* node) {
    /*For debug*/
    fprintf(stderr, "\tPrint AST\n");

    if (node->token == 0 ) {// || node->left == 0 || node->right == 0) {
        return;
    }

    AST* copy = node;
    printf("\tHead token: %d(%s)\n", copy->token->type, copy->token->word);
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

