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
#include "tree_translator/tree_translator.h"
#include "ast.h"

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
// TODO: replace 'exit' with 'status=<ERR>' and check it
/**
 * @brief Read lines from opened file and compiled them
 * @note Abort program if any Error occured
 * @param[in] fd Opened file with program
 * @param[in, out] byte_code Valid ByteArray object from write bytes in
 * @return status OK or Error code
 */
int compile_file(FILE* fd, ByteArray* byte_code) {
    /*Read lines from file and compile it immediately*/
    size_t line_size = DEFAULT_SIZE;
    char* line = calloc(line_size, sizeof(char));

    if ( !line ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Compiler: compile_file: calloc return NULL for 'line' var\n");
        exit(MEM_ERROR);
    }

    int status = OK;
    int read_flag = 0;
    size_t line_count = 0;

    //printf("line addr: %p\n", line);
    while ( !read_flag && !feof(fd) ) {
        //this shit leak  VVVVV
        ssize_t readed = getline(&line, &line_size, fd);
        //printf("line addr loop: %p\n", line);

        if ( !line ) {
            show_error(MEM_ERROR);
            fprintf(stderr, "Compiler: compile_file: getline return NULL\n");
            exit(MEM_ERROR);
        }

        if ( readed == -1 ) {
            read_flag = 1;
            continue;
        }

        replace_f(line, '\n', 0);

        if ( line[0] == 0 ||
             line[0] == COMMENT_SYMBOL ) { // empty line
            line_count++;
            continue;
        }

        replace_f(line, COMMENT_SYMBOL, 0);

        AST* tokens_line = tokenize_line(line, line_count);
        int ret_code = translate_token_tree(tokens_line, 
                                            byte_code->array,
                                            &byte_code->index
                                            );

        if ( ret_code ) {
            show_error(TRANSLATE_LINE_ERR);
            fprintf(stderr, "compile_file: translate tree: invalid line #%zu\n", line_count);
            status = MEM_ERROR;
        }

        if ( !increase_bytearray_size(byte_code) ) {
            show_error(MEM_ERROR);
            fprintf(stderr, "[COMPILER]: increase_bytearray_size: ERROR\n");
            exit(MEM_ERROR);
        }
        memset(line, 0, line_size);
        line_count++;

        if (status) {
            read_flag = 1;
            continue;
        }
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

    // open file and check for exist
    const char* filename = argv[1];
    FILE* fd = fopen(filename, "r");

    if ( !fd ) {
        show_error(FILE_ERROR);
        fprintf(stderr, "Can't open: %s\n", filename);
        perror("With Error: ");
        exit(FILE_ERROR);
    }

    // alloc mem for array with byte code
    ByteArray* bin_code = create_bytearray();

    if ( !bin_code ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "main: bin_code is NULL\n");
        exit(MEM_ERROR);
    }

    int status = OK;

    //read line, tokenize it, append
    status = compile_file(fd, bin_code);

    if ( status != OK ) {
        show_error(status);
        perror("Compiler: main: ");
        exit(status);
    }

    if ( fd ) {
        fclose(fd);
    }

    // TODO: get line -> translate to byte_code as fast as possible
    //      instead collect trees in arr

    // TODO: get out filename from argv (mb parse argv with getopt)

    // TODO: Generate tmp file with:
    //  srand(time(0)); // init rand
    //  num = rand(); 
    //  sprintf(name, "tmp_name_%x", num);
    char* file_out = STD_FILENAME_OUT;
    fd = fopen(file_out, "wb");

    if ( !fd ) {
        show_error(FILE_ERROR);
        fprintf(stderr, "main: fd is null");
        perror("> ");
        exit(FILE_ERROR);
    }

    size_t real_writen = fwrite(bin_code->array, 1, bin_code->index, fd);

    if (real_writen != bin_code->index) {
        fprintf(stderr, "expect write %zu, real writen: %zu\n", bin_code->index, real_writen);
        show_error(FILE_ERROR);
        perror("> ");

    } else {
        real_writen = 0;
    }

    if ( fd ) {
        fclose(fd);
    }

    destroy_bytearray(bin_code);

    if (status) {
        show_error(status);
        fprintf(stderr, "Unknown at: main\n");
    }

    return status;
}
