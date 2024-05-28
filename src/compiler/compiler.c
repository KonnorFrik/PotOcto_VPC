// #include "compiler.h"
// \w\+\s[r]\?\(0x|0b\)\?[0-9a-f]\+\s[r]\?\(0x|0b\)[0-9a-f]\+
#define _GNU_SOURCE
#define __INSTRUCTIONS_PARSE_H__ // turn off including funcs
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../hardware/config.h" 
#include "../common/error_codes.h"
#include "../common/funcs.h"
#include "../str_funcs/str_funcs.h"
#include "../byte_array/byte_array.h" 
#include "tree_translator/tree_translator.h"
#include "ast.h"
#include "keywords_array.h"

#define REG_ACCESS_WORD 'r'
#define MEM_ACCESS_WORD '$'
#define COMMENT_SYMBOL ';'
#define STD_FILENAME_OUT "prog.out"

#define CMP_DEBUG 0

void usage(const char* prog_name);
Token* create_token(int type, char* word, dword value);
int is_line_kw(char* line);
int is_line_access_op(char* line);
int is_line_number(char* line);
dword str_to_num(char* line);
Token* get_token(char* line);
Node* create_node();
int append_to_node(Node* head, Token* token, int token_count);
void fix_new_line(char* line, size_t line_size);
int safe_create_append(char* line, Node* ast, int token_count);
Node* tokenize_line(char* line, size_t line_count);
void print_node(Node* node);
int append_tree(AST* asts_obj, Node* obj);
int compile_file(FILE* fd, ByteArray* byte_code);


void usage(const char* prog_name) {
    printf("Usage: %s file\n", prog_name);
}

Token* create_token(int type, char* word, dword value) {
    Token* obj = calloc(1, sizeof(Token));

   if ( obj ) {
        obj->type = type;
        obj->value = value;
        obj->word = calloc(strlen(word) + 1, sizeof(char));

        if ( obj->word ) {
            strcpy(obj->word, word);
        }
    }

    return obj;
}

int is_line_kw(char* line) {
    int result = OK;
    int ind = 0;

    while (!result && Keywords[ind] != NULL) {
        if (strcmp(line, Keywords[ind]) == 0) {
            result = 1;
        }

        ind++;
    }

    return result;
}

int is_line_access_op(char* line) {
    int result = 0;

    if (line[0] == REG_ACCESS_WORD || line[0] == MEM_ACCESS_WORD) {
        result = 1;
    }

    return result;
}

int is_line_number(char* line) {
    int result = 0;

    if (string_type((const char*)line) != INVALID) {
        result = 1;
    }

    return result;
}

dword str_to_num(char* line) {
    dword result = 0;
    int type = string_type(line);
    int base = 0;

    if (type == BIN) {
        base = 2;
        line += 2;

    } else if (type == HEX) {
        base = 16;
        line += 2;
    }

    result = (dword)strtol(line, NULL, base);

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: str_to_num: '%s':type(%d) -> '%d'\n", line, type, result);
    #endif

    return result;
}

Token* get_token(char* line) {
    int token_type = INVALID;
    char* word = "";
    dword value = 0;

    if (is_line_kw(line)) {
        token_type = KEYWORD;
        word = line;

    } else if (is_line_access_op(line)) {
        if (line[0] == 'r') {
            token_type = REG_ACCESS_OPERATOR;

        } else if (line[0] == '$') {
            token_type = MEM_ACCESS_OPERATOR;
        }

    } else if (is_line_number(line)) {
        token_type = NUMBER;
        value = str_to_num(line);
    }

    Token* token = create_token(token_type, word, value);

    if ( !token ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "get_token: token\n");
    }

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: Create Token: type: %d value: %u\n", token_type, value);
    #endif

    return token;
}

Node* create_node() {
    Node* obj = calloc(1, sizeof(Node));

    if ( obj ) {
        obj->token = NULL;
        obj->left = NULL;
        obj->right = NULL;
    }

    return obj;
}

int append_to_node(Node* head, Token* token, int token_count) {
    if (token_count > 2) {
        return INVALID_LINE;
    }

    if (token->type == INVALID) {
        return INVALID_WORD;
    }

    int result = 0;
    Node* copy = head;

    if (token_count == 0) { //kw
        copy->token = token;
    }

    if (token_count == 1) { //op1
        while (copy->left != NULL) {
            copy = copy->left;
        }

        copy->left = create_node();

        if ( copy->left ) {
            copy->left->token = token;

        } else {
            result = MEM_ERROR; // MEM_ERROR
        }
    }

    if (token_count == 2) { //op2
        while (copy->right != NULL) {
            copy = copy->right;
        }

        copy->right = create_node();

        if ( copy->right ) {
            copy->right->token = token;

        } else {
            result = MEM_ERROR; // MEM_ERROR
        }
    }

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: Append token: token: %d | count: %d\n", token->type, token_count);
    #endif
    
    return result;
}

void fix_new_line(char* line, size_t line_size) {
    /*Replace \n to \0*/
    size_t len = strlen(line);

    if (line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }

    if (len + 1 < line_size) {
        line[len + 1] = '\0';
    }
}

int safe_create_append(char* line, Node* ast, int token_count) {
    /*append token to AST if it not invalid*/
    int result = OK; // 0

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: word: '%s'\n", line);
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

Node* tokenize_line(char* line, size_t line_count) {
    /*Create AST from line*/
    size_t len = strlen(line);
    Node* ast_node = create_node();

    if ( ast_node ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Tokens_line: ast_node is NULL\n");
        exit(MEM_ERROR);
    }

    char* buffer = calloc(len + 2, sizeof(char));
    
    if ( buffer ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Tokens_line: buffer line is NULL\n");
        exit(MEM_ERROR);
    }

    strcpy(buffer, line);

    int err_code = 0;
    int read_flag = 1;
    //TODO replace magic numbers with enum
    int token_count = 0; //0-kw 1-op1 2-op2

    while (read_flag) {
        long unsigned int ind = 0;

        if (buffer[ind] == '\0' || buffer[ind] == '\n') {
            read_flag = 0;
            continue;
        }

        while (buffer[ind] != ' ' && !(buffer[ind] == '\0' || buffer[ind] == '\n')) {
            ind++;
        }

        char save = buffer[ind];
        buffer[ind] = '\0';
        err_code = safe_create_append(buffer, ast_node, token_count);

        #if CMP_DEBUG == 1
            fprintf(stderr, "\t[CMP_DEBUG]: >append code: '%d'\n\n", err_code);
        #endif

        if (err_code) {
            read_flag = 0;
            continue;
        }

        buffer[ind] = save;
        buffer += ++ind;
        token_count++;
    }

    free(buffer);

    if (err_code) {
        show_error(err_code);
        fprintf(stderr, "#%zu > '%s'\n", line_count, line);
        exit(ERROR);

    }

    return ast_node;
}

void print_node(Node* node) {
    /*For debug*/
    if (node->token == 0 || node->left == 0 || node->right == 0) {
        return;
    }

    Node* copy = node;
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

int append_tree(AST* asts_obj, Node* obj) {
    /*Append one ast to array 
     * Potential depricated */
    int status = OK;

    if (asts_obj->index >= asts_obj->size) {
        size_t new_size = (asts_obj->size + (asts_obj->size / 2)) * sizeof(Node*);
        Node** tmp = realloc(asts_obj->array, new_size);

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

int compile_file(FILE* fd, ByteArray* byte_code) {
    /*Read lines from file and compile it immediately*/
    size_t line_size = DEFAULT_SIZE;
    char* line = calloc(line_size, sizeof(char));

    if ( line ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Compiler: compile_file: c_alloc return NULL for 'line' var\n");
        exit(MEM_ERROR);
    }

    int status = OK;
    int read_flag = 0;
    size_t line_count = 0;

    //printf("line addr: %p\n", line);
    while (!read_flag && !feof(fd)) {
        //this shit leak  VVVVV
        ssize_t readed = getline(&line, &line_size, fd);
        //printf("line addr loop: %p\n", line);

        if ( line ) {
            show_error(MEM_ERROR);
            fprintf(stderr, "Compiler: compile_file: getline return NULL\n");
            exit(MEM_ERROR);
        }

        if (readed == -1) {
            read_flag = 1;
            continue;
        }

        fix_new_line(line, line_size);

        if (line && line[0] == 0) { // empty line
            line_count++;
            continue;
        }

        line_size = replace(line, COMMENT_SYMBOL, 0);

        if (line_size == 0) { //all line is comment
            continue;
        }

        Node* tokens_line = tokenize_line(line, line_count);
        int ret_code = translate_token_tree(tokens_line, 
                                            byte_code->array,
                                            &byte_code->index
                                            );

        if (ret_code) {
            show_error(TRANSLATE_LINE_ERR);
            fprintf(stderr, "compile_file: translate tree: invalid line #%zu\n", line_count);
            status = MEM_ERROR;
        }

        increase_bytearray_size(byte_code);
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

    if ( fd ) {
        show_error(FILE_ERROR);
        fprintf(stderr, "Can't open: %s\n", filename);
        perror("With Error: ");
        exit(FILE_ERROR);
    }

    // alloc mem for array with AST's
    //size_t trees_array_size = 30;
    //Node** trees_array = c_alloc(trees_array_size, sizeof(Node*));
    //size_t trees_array_index = 0;

    //if (is_null(trees_array)) {
        //show_error(MEM_ERROR);
        //fprintf(stderr, "main: trees_array\n");
        //full_exit(MEM_ERROR);
    //}

    // alloc mem for array with byte code
    ByteArray* bin_code = create_bytearray();

    if ( bin_code ) {
        show_error(MEM_ERROR);
        fprintf(stderr, "main: bin_code is NULL\n");
        exit(MEM_ERROR);
    }

    int status = OK;

    //read line, tokenize it, append
    status = compile_file(fd, bin_code);

    if (status) {
        show_error(status);
        perror("Compiler: main: ");
        exit(status);
    }

    if ( fd ) {
        fclose(fd);
    }

    //int translate_code = OK;
    //size_t current_tree = 0;

    // iterate for AST's and compile it
    //for (; translate_code == OK && current_tree < trees_array_index; ++current_tree) {
        //translate_code = translate_token_tree(trees_array[current_tree], bin_code, &bin_code_index);
    //}

    //#if CMP_DEBUG == 1
        //fprintf(stderr, "\t[CMP_DEBUG]: translate code: %d\n", translate_code);
        //fprintf(stderr, "\t[CMP_DEBUG]: Byte code:\n\t");
//
        //for (size_t i = 0; i < bin_code_index; ++i) {
            //if ((i % 3) == 0) {
                //fprintf(stderr, "\n\t");
            //}
//
            //fprintf(stderr, "%x ", bin_code[i]);
        //}
//
        //fprintf(stderr, "\n");
    //#endif

    //if (translate_code) {
        //show_error(TRANSLATE_LINE);
        //fprintf(stderr, "main: translate tree: valid line #%zu\n", current_tree + 0);
        //full_exit(TRANSLATE_LINE);
    //}

    //TODO get line -> translate to byte_code as fast as possible
    //      instead collect trees in arr

    //TODO get out filename from argv (mb parse argv with getopt)

    //TODO Generate tmp file with:
    //  srand(time(0)); // init rand
    //  num = rand(); 
    //  sprintf(name, "tmp_name_%x", num);
    char* file_out = STD_FILENAME_OUT;
    fd = fopen(file_out, "wb");

    if ( fd ) {
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
