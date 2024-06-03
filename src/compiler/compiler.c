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
#include "../str_funcs/str_funcs.h"
#include "../byte_array/byte_array.h" 
#include "lexer/lexer.h"
#include "tree_translator/tree_translator.h"

#define STD_FILENAME_OUT "prog.out"

#define CMP_DEBUG 1

void usage(const char* prog_name);

/**
 * @brief Read lines from opened file and compiled them
 * @param[in] fd Opened file with program
 * @param[in, out] byte_code Valid ByteArray object from write bytes in
 * @return status OK or Error code
int compile_file(FILE* fd, ByteArray* byte_code);
 */

/**
 * @brief Print Usage of compiler to stdout
 * @param[in] prog_name Current path to this program
 * @return void
 */
void usage(const char* prog_name) {
    printf("Usage: %s file\n", prog_name);
}

// TODO: Change appendig 'system' to node. Head is Keyword, append left operand1, append right operand2
// Appending to AST use token_count for determine append token to left or right
// Language(asm like) have only 1-3 words per line
// 1 word - Keyword
// 2 word - Left operand
// 3 word - Right operand


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
        replace_f(line, COMMENT_SYMBOL, 0);

#if CMP_DEBUG == 1
        fprintf(stderr, "\n\n[CMP_DEBUG]: Read line #%zu, '%s'\n", line_count, line);
#endif

        if ( line[0] == 0 ) {
            continue;
        }

        AST* tokens_line = ast_tokenize_line(line);

#if CMP_DEBUG == 1
        fprintf(stderr, "[CMP_DEBUG]: tokens_line addr: %p\n", (void*)tokens_line);
        fprintf(stderr, "AST:\n");
        ast_print(tokens_line);
        fprintf(stderr, "\n");
#endif

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
            continue;
        }

        status = translate_token_tree(tree->array[ind], bin_code->array, &bin_code->index);
#if CMP_DEBUG == 1
        fprintf(stderr, "[CMP_DEBUG]: Translate index: %zu  status: %d\n", ind, status);
#endif
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
#endif

    if ( status == OK ) {
        bin_code = translate_tree(ast_tree);

        if ( !bin_code ) {
            status = ERROR;
        }
    }

#if CMP_DEBUG == 1
    fprintf(stderr, "[CMP_DEBUG]: Status after translate: %d\n", status);
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

/*
int compile_file(FILE* fd, ByteArray* byte_code) {
    if ( !fd || !byte_code ) {
        return MEM_ERROR;
    }

    int status = OK;
    size_t line_size = DEFAULT_SIZE;
    char* line = calloc(line_size, sizeof(char));

    if ( !line ) {
        status = MEM_ERROR;
#if CMP_DEBUG == 1
        fprintf(stderr, "[compile_file]: line is NULL\n");
#endif
    }

    size_t line_count = 0;

    //printf("line addr: %p\n", line);
    while ( (status == OK) && !feof(fd) ) {
        ssize_t readed = getline(&line, &line_size, fd);

        if ( !line ) {
            status = MEM_ERROR;
        }

        if ( readed == -1 ) { // same condition as feof(fd)
#if CMP_DEBUG == 1
            fprintf(stderr, "[compile_file]: readed is -1\n");
#endif
            continue;
        }

        line_count++;

        replace_f(line, '\n', 0);
        replace_f(line, COMMENT_SYMBOL, 0);

        if ( line[0] == 0 ) {
            continue;
        }

        AST* tokens_line = ast_tokenize_line(line);

        if ( !tokens_line ) {
            fprintf(stderr, "[SYNTAX ERROR]: Can't tokenize line: '%s'\n", line);
            fprintf(stderr, "[SYNTAX ERROR]: Line No: %zu\n", line_count);
            status = SYNTAX_ERR;
            continue;
        }

        int ret_code = translate_token_tree(tokens_line, 
                                            byte_code->array,
                                            &byte_code->index
                                            );

        if ( ret_code ) {
            fprintf(stderr, "[SYNTAX ERROR]: Can't translate line: '%s'\n", line);
            fprintf(stderr, "[SYNTAX ERROR]: Line No: %zu\n", line_count);
            status = SYNTAX_ERR;
            continue;
        }

        if ( !bytearray_increase_size(byte_code) ) {
            status = MEM_ERROR;
#if CMP_DEBUG == 1
            fprintf(stderr, "[compile_file]: can't increase ByteArray\n");
#endif
        }

        memset(line, 0, line_size);
    }

    if ( line ) {
        free(line);
    }

    return status;
}

int main(const int argc, const char** argv) {
    if (argc < 2) {
        usage(argv[0]);
        exit(WRONG_ARGS);
    }

    int status = OK;

    // open file and check for exist
    const char* filename = argv[1];
    FILE* fd = fopen(filename, "r");

    if ( !fd ) {
        status = FILE_ERROR;
    }

    // alloc mem for array with byte code
    ByteArray* bin_code = NULL;

    if ( status == OK ) {
        bin_code = create_bytearray();

        if ( !bin_code ) {
            status = MEM_ERROR;
        }
    }

    //read line, tokenize it, append
    if ( status == OK ) status = compile_file(fd, bin_code);

    if ( fd ) {
        fclose(fd);
        fd = NULL;
    }

    // TODO: get out filename from argv (mb parse argv with getopt)

    char* file_out = STD_FILENAME_OUT;

    if ( status == OK ) {
        fd = fopen(file_out, "wb");

        if ( !fd ) {
            status = FILE_ERROR;
        }
    }

    size_t real_writen = 0;
    if ( status == OK ) real_writen = fwrite(bin_code->array, 1, bin_code->index, fd);
    if ( status == OK ) printf("Expect write %zu\nReal writen: %zu\n", bin_code->index, real_writen);
    if ( fd ) {
        fclose(fd);
        fd = NULL;
    }

    if ( bin_code ) destroy_bytearray(bin_code);

    if (status) {
        fprintf(stderr, "Error occured, type: ");
        show_error(status);
    }

    return status;
}
*/
