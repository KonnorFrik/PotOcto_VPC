// every line match this pattern.
// \w\+\s[r]\?\(0x|0b\)\?[0-9a-f]\+\s[r]\?\(0x|0b\)[0-9a-f]\+
#define _GNU_SOURCE
#define __INSTRUCTIONS_PARSE_H__ // turn off including funcs
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// #include "../hardware/basic_types.h" 
#include "../common/error_codes.h"
// #include "../common/funcs.h"
// #include "../str_funcs/str_funcs.h"
#include "../byte_array/byte_array.h" 
#include "lexer/lexer.h"
#include "tree_translator/tree_translator.h"

#define STD_FILENAME_OUT "prog.out"

#define CMP_DEBUG 1

typedef struct ast_arr {
    AST** array;
    size_t index;
    size_t size; ///< count of cells
} AST_ARR;

AST_ARR* astarr_create();
void astarr_destroy(AST_ARR* obj);
int astarr_append(AST_ARR* arr, AST* tokens);
void usage(const char* prog_name);
bool replace_f(char* line, char from, char to);

bool replace_f(char* line, char from, char to) {
    size_t ind = 0;
    bool loop = true;

    while (loop && line[ind] != 0) {
        if (line[ind] == from) {
            line[ind] = to;
            loop = false;
            continue;
        }

        ind++;
    }

    return !loop;
}

AST_ARR* astarr_create() {
    AST_ARR* obj = calloc(1, sizeof(AST_ARR));

    if ( obj ) {
        obj->size = 1;
        obj->array = calloc(1, sizeof(AST*));

        if ( !obj->array ) {
            free(obj);
            obj = NULL;
        }
    }

    return obj;
}

int astarr_append(AST_ARR* arr, AST* tokens) {
    if ( !arr || !tokens ) {
        return MEM_ERROR;
    }

    int status = OK;

    if ( arr->index >= arr->size ) {
        size_t new_size = (arr->size + (arr->size / 2)) + 1;
#if CMP_DEBUG == 1
        fprintf(stderr, "[CMP_DEBUG]: realloc AST_ARR->array:\n");
        fprintf(stderr, "[CMP_DEBUG]: \tindex:     %zu\n", arr->index);
        fprintf(stderr, "[CMP_DEBUG]: \tprev size: %zu bytes\n", arr->size);
        fprintf(stderr, "[CMP_DEBUG]: \tnew  size: %zu cells\n", new_size);
        fprintf(stderr, "[CMP_DEBUG]: \tnew  size: %zu bytes\n", new_size * sizeof(AST*));
#endif

        AST** tmp = realloc(arr->array, new_size * sizeof(AST*));

        if ( tmp ) {
            arr->array = tmp;
            arr->size = new_size;

        } else { 
            status = MEM_ERROR;
        }
    }

    if ( status == OK ) {
        arr->array[arr->index++] = tokens;
    }

    return status;
}

void astarr_destroy(AST_ARR* obj) {
    if ( !obj ) {
        return;
    }

    if ( obj->array ) {
        for (size_t i = 0; i < obj->index; ++i ) {
            if ( obj->array[i] ) {
                ast_destroy(obj->array[i]);
            }
        }
    }

    free(obj);
}

/**
 * @brief Print Usage of compiler to stdout
 * @param[in] prog_name Current path to this program
 * @return void
 */
void usage(const char* prog_name) {
    printf("Usage: %s file\n", prog_name);
}


AST_ARR* tokenize_file(FILE* file) {
    int status = OK;
    AST_ARR* tree = astarr_create();

    if ( !tree ) {
#if CMP_DEBUG == 1
        fprintf(stderr, "[CMP_DEBUG]: Can't allocate tree \n");
#endif
        status = MEM_ERROR;
    }

    char* line = NULL;
    size_t line_size = 0;
    size_t line_count = 0;

    while ( (status == OK) && !feof(file) ) {
        ssize_t readed = getline(&line, &line_size, file);

        if ( !line ) {
            status = MEM_ERROR;
            continue;
        }

        if ( readed == -1 ) {
            continue;
        }

        line_count++;
        replace_f(line, '\n', 0);

#if CMP_DEBUG == 1
        fprintf(stderr, "\n\n[CMP_DEBUG]: Read line #%zu, '%s'\n", line_count, line);
#endif

        if ( line[0] == 0 ) {
            continue;
        }

        AST* tokens_line = lexer_tokenize_line(line);

        if ( !tokens_line ) {
            fprintf(stderr, "[SYNTAX ERROR]: Can't tokenize line number: %zu\n", line_count);
            fprintf(stderr, "\tline: '%s'\n", line);
            status = SYNTAX_ERR;
            continue;

        }

        status = astarr_append(tree, tokens_line);

#if CMP_DEBUG == 1
        fprintf(stderr, "[CMP_DEBUG]: Append status: %d\n", status);
#endif
    }

    return tree;
}

ByteArray* translate_tree(AST_ARR* tree) {
    int status = OK;
    ByteArray* bin_code = bytearray_create();

    if ( !bin_code ) {
#if CMP_DEBUG == 1
        fprintf(stderr, "[CMP_DEBUG]: Can't allocate ByteArray\n");
#endif
        status = MEM_ERROR;
    }

    size_t ind = 0;

    for (; (status == OK) && ind < tree->index; ++ind) {
        if ( tree->array[ind]->token->type == COMMENT ) {
#if CMP_DEBUG == 1
            fprintf(stderr, "\t[CMP_DEBUG]: Find comment: SKIP\n");
#endif
            continue;
        }

        if ( tree->array[ind]->token->type == UNKNOWN ) {
#if CMP_DEBUG == 1
            fprintf(stderr, "\t[CMP_DEBUG]: Find unknown: SKIP\n");
#endif
            continue;
        }

        status = translate_token_tree(tree->array[ind], bin_code->array, &bin_code->index);
#if CMP_DEBUG == 1
        fprintf(stderr, "[CMP_DEBUG]: Translated index: %zu  status: %d\n", ind, status);
#endif

        if ( status == OK ) {
            status = bytearray_increase_size(bin_code);
#if CMP_DEBUG == 1
            fprintf(stderr, "[CMP_DEBUG]: Try increase ByteArray, status: %d\n", status);
#endif
        } else {
            fprintf(stderr, "Translate ERROR: at line: %zu\n", ind + 1);
        }
    }

    if ( status != OK ) {
        bytearray_destroy(bin_code);
        bin_code = NULL;
    }

    return bin_code;
}

void write_bin_code(ByteArray* bin_code, FILE* file) {
    size_t real_writen = fwrite(bin_code->array, 1, bin_code->index, file);
    printf("Expect write %zu\n", bin_code->index);
    printf("Real writen: %zu\n", real_writen);
}

int main(const int argc, const char** argv) {
    if ( argc < 2 ) {
        usage(argv[0]);
        return WRONG_ARGS;
    }

    int status = OK;

    FILE* file_in = NULL;
    FILE* file_out = NULL;
    AST_ARR* ast_tree = NULL;
    ByteArray* bin_code = NULL;

    if ( !(file_in = fopen(argv[1], "r")) ) {
        status = FILE_ERROR;
    }

    if ( status == OK ) {
        ast_tree = tokenize_file(file_in);

        if ( !ast_tree ) {
            status = ERROR;
        }
    }

#if CMP_DEBUG == 1
    fprintf(stderr, "[CMP_DEBUG]: Status after tokenize: %d\n", status);
    fprintf(stderr, "\t[CMP_DEBUG]: Tokenize lines: %zu\n", ast_tree->index);
    fprintf(stderr, "\n");
#endif

    if ( status == OK ) {
        bin_code = translate_tree(ast_tree);

        if ( !bin_code ) {
            status = ERROR;
        }
    }

#if CMP_DEBUG == 1
    fprintf(stderr, "[CMP_DEBUG]: Status after translate: %d\n", status);
    fprintf(stderr, "\t[CMP_DEBUG]: Translate bytes: %zu\n", bin_code ?  bin_code->index : 0);
    fprintf(stderr, "\n");
#endif

    char* filename = STD_FILENAME_OUT;

    if ( status == OK ) {
        if ( argc >= 3) {
            filename = (char*)argv[2];

        }

        file_out = fopen(filename, "wb");
    }

#if CMP_DEBUG == 1
    fprintf(stderr, "[CMP_DEBUG]: filename out: %s\n", filename);
    fprintf(stderr, "[CMP_DEBUG]: fd file out: %p\n", (void*)file_out);
    fprintf(stderr, "[CMP_DEBUG]: Status after open file out: %d\n", status);
#endif

    if ( status == OK ) {
        if ( file_out ) {
            write_bin_code(bin_code, file_out);

        } else {
            status = FILE_ERROR;
        }
    }

#if CMP_DEBUG == 1
    fprintf(stderr, "[CMP_DEBUG]: Status after write compiled code: %d\n", status);
#endif

    if ( ast_tree ) {
        astarr_destroy(ast_tree);
    }

    if ( bin_code ) {
        bytearray_destroy(bin_code);
    }

    if ( file_in ) {
        fclose(file_in);
    }

    if ( file_out ) {
        fclose(file_out);
    }

    return status;
}

