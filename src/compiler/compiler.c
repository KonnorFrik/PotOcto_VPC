// every line match this pattern.
// \w\+\s[r]\?\(0x|0b\)\?[0-9a-f]\+\s[r]\?\(0x|0b\)[0-9a-f]\+
#define _GNU_SOURCE
#define __INSTRUCTIONS_PARSE_H__ // turn off including funcs
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../hardware/basic_types.h" 
#include "../common/error_codes.h"
#include "../common/funcs.h"
#include "../str_funcs/str_funcs.h"
#include "../byte_array/byte_array.h" 
#include "lexer/lexer.h"
#include "tree_translator/tree_translator.h"

#define COMMENT_SYMBOL ';'
#define STD_FILENAME_OUT "prog.out"

#define CMP_DEBUG 0

void usage(const char* prog_name);

int compile_file(FILE* fd, ByteArray* byte_code);

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

// TODO: find way to split function to smaller function
/**
 * @brief Read lines from opened file and compiled them
 * @param[in] fd Opened file with program
 * @param[in, out] byte_code Valid ByteArray object from write bytes in
 * @return status OK or Error code
 */
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

        replace_f(line, COMMENT_SYMBOL, 0);
        AST* tokens_line = tokenize_line(line);

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

        if ( !increase_bytearray_size(byte_code) ) {
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
