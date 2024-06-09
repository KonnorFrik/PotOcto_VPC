// every line match this pattern.
// \w\+\s[r]\?\(0x|0b\)\?[0-9a-f]\+\s[r]\?\(0x|0b\)[0-9a-f]\+
#define _GNU_SOURCE
#define __INSTRUCTIONS_PARSE_H__ // turn off including funcs
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "../common/error_codes.h"
#include "../byte_array/byte_array.h" 
#include "lexer/lexer.h"
#include "tree_translator/tree_translator.h"
#include "options.h"

#define STD_FILENAME_OUT "prog.out"

#define CMP_DEBUG 0
#define ARG_PRINT_AST "print_ast"
#define ARG_HELP "help"
#define ARG_VERBOSE "verbose"

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
    printf("Options:\n");

    printf("%20s: - Print ast after tokenize line\n", ARG_PRINT_AST);
    printf("%20s: - Show verbose compilation info\n", ARG_VERBOSE);
    printf("%20s: - Show this help\n", ARG_HELP);
}


AST_ARR* tokenize_file(FILE* file, Options* opt) {
    if ( !file || !opt ) {
        return NULL;
    }

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

        if ( line[0] == WORD_COMMENT ) { // skip comment, do not append its to ast
            if ( opt->verbose ) {
                printf("[COMPILER]: Discard full-comment line\n");
            }

            continue;
        }

        line_count++;
        replace_f(line, '\n', 0);

        if ( opt->verbose ) {
            fprintf(stderr, "[COMPILER]: Read line #%zu, '%s'\n", line_count, line);
        }

        if ( line[0] == 0 ) {
            continue;
        }

        AST* tokens_line = lexer_tokenize_line(line, opt);

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

ByteArray* translate_tree(AST_ARR* tree, Options* opt) {
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

        status = translate_token_tree(tree->array[ind], bin_code->array, &bin_code->index, opt);
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

int write_bin_code(ByteArray* bin_code, FILE* file) {
    if ( !bin_code || !file ) {
        fprintf(stderr, "Error on writing");
        return WRONG_ARGS;
    }

    size_t real_writen = fwrite(bin_code->array, 1, bin_code->index, file);
    printf("Expect write %zu bytes\n", bin_code->index);
    printf("Real writen: %zu bytes\n", real_writen);
    return OK;
}

int parse_args(int argc, char** argv, Options* opt) {
    int status = OK;

    const char* short_opt = "A";
    const struct option long_opt[] = {
        {ARG_PRINT_AST, no_argument, &opt->lexer_show_ast, true},
        {ARG_HELP, no_argument, &opt->help, true},
        {ARG_VERBOSE, no_argument, &opt->verbose, true},
    };

    while ( getopt_long(argc, argv, short_opt, long_opt, NULL) != -1 );

    return status;
}

/**
* @brief Iterate over every node for find label with given name 
* @param[in] ast_tree Tokenized AST_ARR object
* @param[in] line String with label name for search (with ':')
* @param[out] index With ast_tree valid index 
* @return result Is found(true) or not(false)
*/
bool find_lbl_start(AST_ARR* ast_tree, char* line, size_t* lbl_ind, Options* opt) {
    if ( !ast_tree || !line || !lbl_ind ) {
        return false;
    }

    bool result = false;
    size_t addr = 0;
    size_t len = strlen(line); //ignore ':' at the end

    for (size_t i = 0; i < ast_tree->index && !result ; ++i) {
        AST* node = ast_tree->array[i];

        if ( node->token->type != LABEL_START && node->token->type != LABEL_END ) {
            addr += 3;
        }

        while ( ( node != NULL ) && !result ) {
            if ( node->token->type == LABEL_START &&
                ( (strlen(node->token->line) - 1) == len ) &&
                ( strncmp(line, node->token->line, len) == 0 ) ) {
                *lbl_ind = addr;
                result = true;

                if ( opt->verbose ) {
                    fprintf(stderr, "\t[COMPILER]: Found LABEL_END at: %lu\n", i);
                    fprintf(stderr, "\t[COMPILER]: with line: '%s'\n", node->token->line);
                    fprintf(stderr, "\t[COMPILER]: with addr: %lu\n", addr);
                }
            }

            node = node->next;
        }
    }

    return result;
}

/**
* @brief Iterate over every ast node, find labels like 'lbl:' and substitute name with address from label like 'lbl' if exist
* @param[in, out] ast_tree Tokenized AST_ARR object
* @return status Code from 'enum error_codes'
*/
int substitute_labels(AST_ARR* ast_tree, Options* opt) {
    if ( !ast_tree ) {
        return WRONG_ARGS;
    }

    int status = OK;

    for (size_t i = 0; i < ast_tree->index; ++i) {
        int is_lbl_found = 0;
        int inner_loop = 1;
        AST* node = ast_tree->array[i];

        while ( ( node != NULL ) && inner_loop && !is_lbl_found ) {
            if ( node->token->type == COMMENT ) {
                inner_loop = 0; // skip all next nodes because its comments
                continue;
            }

            if ( node->token->type == UNKNOWN ) {
                is_lbl_found = 1;

                if ( opt->verbose ) {
                    fprintf(stderr, "[COMPILER]: Found LABEL_START at: %lu\n", i);
                    fprintf(stderr, "\t[COMPILER]: label for search: '%s'\n", node->token->line);
                }
                
                continue;
            }

            node = node->next;
        }

        if ( is_lbl_found ) {
            size_t lbl_ind = 0;

            if ( find_lbl_start(ast_tree, node->token->line, &lbl_ind, opt) ) {
                node->token->type = LABEL_END;
                node->token->value = lbl_ind; // convert id to address

                if ( opt->verbose ) {
                    fprintf(stderr, "\t[CMP_DEBUG]: Converted Address: %u\n", node->token->value);
                }

            } else {
                status = ERROR;
            }

        }

    }

    return status;
}

int main(const int argc, char** argv) {
    int status = OK;

    Options user_options = {0};
    status = parse_args(argc, argv, &user_options);
    int file_index = optind;

    if ( file_index >= argc || user_options.help ) {
        usage(argv[0]);
        status = WRONG_ARGS;
    }

    FILE* file_in = NULL;
    FILE* file_out = NULL;
    AST_ARR* ast_tree = NULL;
    ByteArray* bin_code = NULL;

    if ( (status == OK) && (file_in = fopen(argv[file_index], "r")) == NULL ) {
        status = FILE_ERROR;
        fprintf(stderr, "File not exist: %s\n", argv[file_index]);
    }

    if ( user_options.verbose ) {
        printf("\t[COMPILER]Open file: %s\n", argv[file_index]);
        printf("\t[COMPILER]: status after open: %d\n\n", status);
    }

    if ( status == OK ) {
        ast_tree = tokenize_file(file_in, &user_options);

        if ( !ast_tree ) {
            status = ERROR;
        }
    }

    if ( user_options.verbose ) {
        fprintf(stderr, "\t[COMPILER]: Tokenized lines: %zu\n", ast_tree ? ast_tree->index : 0);
        fprintf(stderr, "\t[COMPILER]: Status after tokenize: %d\n", status);
        fprintf(stderr, "\n");
    }

    if ( status == OK ) {
        status = substitute_labels(ast_tree, &user_options);
    }

    if ( status == OK ) {
        bin_code = translate_tree(ast_tree, &user_options);

        if ( !bin_code ) {
            status = ERROR;
        }
    }

    if ( user_options.verbose ) {
        fprintf(stderr, "\t[COMPILER]: Translated bytes: %zu\n", bin_code ?  bin_code->index : 0);
        fprintf(stderr, "\t[COMPILER]: Status after translate: %d\n", status);
        fprintf(stderr, "\n");
    }

    char* filename = STD_FILENAME_OUT;

    if ( status == OK ) {
        if ( file_index + 1 < argc ) {
            filename = (char*)argv[file_index + 1];

        }

        file_out = fopen(filename, "wb");

        if ( !file_out ) {
            status = FILE_ERROR;
        }
    }

    if ( user_options.verbose ) {
        fprintf(stderr, "\t[COMPILER]: filename out: %s\n", filename);
        fprintf(stderr, "\t[COMPILER]: Status after open: %d\n\n", status);
    }

    if ( status == OK ) {
        status = write_bin_code(bin_code, file_out);
    }

    if ( user_options.verbose ) {
        fprintf(stderr, "[COMPILER]: Status after write compiled code: %d\n", status);
    }

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

