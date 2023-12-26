#include "compiler.h"
// \w\+\s[r\$]\?\(0x|0b\)\?[0-9a-f]\+\s[r\$]\?\(0x|0b\)[0-9a-f]\+

void full_exit(int code) {
    m_destroy();
    exit(code);
}

void usage(const char* prog_name) {
    printf("Usage: %s file\n", prog_name);
}

void show_error(int code) {
    if (code & MEM_ERROR) {
        fprintf(stderr, "Memory Alloc Error\n");

    } else if (code & FILE_ERROR) {
        fprintf(stderr, "File Error\n");

    } else if (code & SYNTAX) {
        fprintf(stderr, "Syntax Error\n");

    } else if (code & INVALID_LINE) {
        fprintf(stderr, "Invalid line Error\n");

    } else if (code & INVALID_WORD) {
        fprintf(stderr, "Invalid word Error\n");

    } else if (code & TRANSLATE_LINE) {
        fprintf(stderr, "Translation Error\n");

    } else {
        fprintf(stderr, "Error\n");
    }
    
}

Token* create_token(int type, char* word, dword value) {
    Token* obj = c_alloc(1, sizeof(Token));

    if (!is_null(obj)) {
        obj->type = type;
        obj->value = value;
        obj->word = c_alloc(strlen(word) + 1, sizeof(char));

        if (!is_null(obj->word)) {
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

    if (is_null(token)) {
        show_error(MEM_ERROR);
        fprintf(stderr, "get_token: token\n");
    }

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: Create Token: type: %d value: %u\n", token_type, value);
    #endif

    return token;
}

Node* create_node() {
    Node* obj = c_alloc(1, sizeof(Node));

    if (!is_null(obj)) {
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

        if (!is_null(copy->left)) {
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

        if (!is_null(copy->right)) {
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
    size_t len = strlen(line);

    if (line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }

    if (len + 1 < line_size) {
        line[len + 1] = '\0';
    }
}

int safe_create_append(char* line, Node* ast, int token_count) {
    int result = OK; // 0

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: word: '%s'\n", line);
    #endif

    Token* word_token = get_token(line);

    if (word_token->type == INVALID) {
        result = SYNTAX;
    }

    result |= append_to_node(ast, word_token, token_count);

    if (word_token->type == MEM_ACCESS_OPERATOR || word_token->type == REG_ACCESS_OPERATOR) {
        result |= safe_create_append(line + 1, ast, token_count);
    }

    return result;
}

Node* tokenize_line(char* line, size_t line_count) {
    size_t len = strlen(line);
    Node* ast_node = create_node();

    if (is_null(ast_node)) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Tokens_line: ast_node\n");
        full_exit(MEM_ERROR);
    }

    char* buffer = c_alloc(len + 2, sizeof(char));
    
    if (is_null(buffer)) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Tokens_line: buffer line\n");
        full_exit(MEM_ERROR);
    }

    strcpy(buffer, line);

    int err_code = 0;
    int read_flag = 1;
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

    m_free(buffer);

    if (err_code) {
        show_error(err_code);
        fprintf(stderr, "#%zu > '%s'\n", line_count, line);
        full_exit(ERROR);

    }// else {
        //post_process_special_words(ast_node, line);
    //}

    return ast_node;
}

void print_node(Node* node) {
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

int append_tree(Node*** arr_addr, size_t* size, size_t* index, Node* obj) {
    int status = OK;

    if (*index >= *size) {
        size_t new_size = (*size + (*size / 2)) * sizeof(Node*);
        Node** tmp = re_alloc(*arr_addr, new_size);

        if (!is_null(tmp)) {
            *arr_addr = tmp;
            *size = new_size;

        } else {
            status = MEM_ERROR;
        }
    }

    (*arr_addr)[(*index)++] = obj;

    return status;
}

void split_comment(char* line, size_t* size) {
    int splitted = 0;
    int i = 0;

    while (!splitted && line[i] != '\0') {
        if (line[i] == COMMENT_SYMBOL) {
            line[i] = '\0';
            splitted = 1;
        }

        i++;
    }

    if (splitted) {
        *size = --i;
    }
}

int compile_file(FILE* fd, Node*** trees_array, size_t* trees_array_size, size_t* trees_array_index, size_t* line_count_out) {
    size_t line_size = 100;
    char* line = c_alloc(line_size, sizeof(char));

    if (is_null(line)) {
        show_error(MEM_ERROR);
        fprintf(stderr, "Compiler: main: c_alloc\n");
        full_exit(MEM_ERROR);
    }

    int status = OK;
    int read_flag = 0;
    size_t line_count = 0;

    while (!read_flag && !feof(fd)) {
        ssize_t readed = getline(&line, &line_size, fd);

        if (is_null(line)) {
            show_error(MEM_ERROR);
            fprintf(stderr, "Compiler: main: getline return NULL\n");
            full_exit(MEM_ERROR);
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

        split_comment(line, &line_size);

        if (line_size == 0) { //all line is comment
            continue;
        }

        Node* tokens_line = tokenize_line(line, line_count);
        status = append_tree(trees_array, trees_array_size, trees_array_index, tokens_line);

        if (status) {
            read_flag = 1;
            continue;
        }

        memset(line, 0, line_size);
        line_count++;
    }

    *line_count_out = line_count;

    if (!is_null(line)) {
        m_free(line);
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

    if (is_null(fd)) {
        show_error(FILE_ERROR);
        fprintf(stderr, "Can't open: %s\n", filename);
        perror("With Error: ");
        full_exit(FILE_ERROR);
    }

    // alloc mem for array with AST's
    size_t trees_array_size = 30;
    Node** trees_array = c_alloc(trees_array_size, sizeof(Node*));
    size_t trees_array_index = 0;

    if (is_null(trees_array)) {
        show_error(MEM_ERROR);
        fprintf(stderr, "main: trees_array\n");
        full_exit(MEM_ERROR);
    }

    int status = OK;
    size_t line_count = 0;

    //read line, tokenize it, append
    status = compile_file(fd, &trees_array, &trees_array_size, &trees_array_index, &line_count);

    if (status) {
        show_error(status);
        perror("Compiler: main: ");
        full_exit(status);
    }

    if (!is_null(fd)) {
        fclose(fd);
    }

    // alloc mem for array with byte code
    size_t bin_code_size = trees_array_index * 3; // 3 for 1.instruction, 2.operand_1, 3.operand_2
    size_t bin_code_index = 0;
    byte* bin_code = c_alloc(bin_code_size, sizeof(byte));

    if (is_null(bin_code)) {
        show_error(MEM_ERROR);
        fprintf(stderr, "main: bin_code\n");
        full_exit(MEM_ERROR);
    }

    #if CMP_DEBUG == 1
        fprintf(stderr, "\n");
        fprintf(stderr, "\t[CMP_DEBUG]: Tokens count: %zu\n", trees_array_index);
        fprintf(stderr, "\t[CMP_DEBUG]: Result in bytes expect: %zu\n", bin_code_size);
        fprintf(stderr, "\t[CMP_DEBUG]: Tokens:\n");
        for (size_t i = 0; i < trees_array_index; ++i) {
            print_node(trees_array[i]);
        }
    #endif

    int translate_code = OK;
    size_t current_tree = 0;

    // iterate for AST's and compile it
    for (; translate_code == OK && current_tree < trees_array_index; ++current_tree) {
        translate_code = translate_token_tree(trees_array[current_tree], bin_code, &bin_code_index);
    }

    #if CMP_DEBUG == 1
        fprintf(stderr, "\t[CMP_DEBUG]: translate code: %d\n", translate_code);
        fprintf(stderr, "\t[CMP_DEBUG]: Byte code:\n\t");

        for (size_t i = 0; i < bin_code_index; ++i) {
            if ((i % 3) == 0) {
                fprintf(stderr, "\n\t");
            }

            fprintf(stderr, "%x ", bin_code[i]);
        }

        fprintf(stderr, "\n");
    #endif

    if (translate_code) {
        show_error(TRANSLATE_LINE);
        fprintf(stderr, "main: translate tree: valid line #%zu\n", current_tree + 0);
        full_exit(TRANSLATE_LINE);
    }

    //TODO get line -> translate to byte_code as fast as possible
    //      instead collect trees in arr

    //TODO get out filename from argv (mb parse argv with getopt)
    char* file_out = "prog.out";
    fd = fopen(file_out, "wb");

    if (is_null(fd)) {
        show_error(FILE_ERROR);
        fprintf(stderr, "main: fd is null");
        perror("> ");
        full_exit(FILE_ERROR);
    }

    size_t real_writen = fwrite(bin_code, 1, bin_code_index, fd);

    if (real_writen != bin_code_index) {
        fprintf(stderr, "expect write %zu, real writen: %zu\n", bin_code_index, real_writen);
        show_error(FILE_ERROR);
        perror("> ");

    } else {
        real_writen = 0;
    }

    if (!is_null(fd)) {
        fclose(fd);
    }

    if (!is_null(trees_array)) {
        m_free(trees_array);
    }

    //if (!is_null(line)) {
        //m_free(line);
    //}

    if (status) {
        show_error(status);
        fprintf(stderr, "main\n");
    }

    m_destroy();
    return OK;
}