#include "compiler.h"
// \w\+\s[r\$]\?\(0x|0b\)\?[0-9a-f]\+\s[r\$]\?\(0x|0b\)[0-9a-f]\+

void usage(const char* prog_name) {
    printf("Usage: %s file\n", prog_name);
}

Token* create_token(int type, dword value) {
    Token* obj = c_alloc(1, sizeof(Token));

    if (!is_null(obj)) {
        obj->type = type;
        obj->value = value;
    }

    return obj;
}

int is_line_kw(char* line) {
    int result = 0;

    if (strcmp(line, "mov") == 0) {
        result = 1;
    }

    return result;
}

int is_line_access_op(char* line) {
    int result = 0;

    if (line[0] == 'r' || line[0] == '$') {
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

    } else if (type == HEX) {
        base = 16;
    }

    result = (dword)strtol(line, NULL, base);

    return result;
}

Token* get_token(char* line) {
    int token_type = INVALID;
    dword value = 0;

    if (is_line_kw(line)) {
        token_type = KEYWORD;

    } else if (is_line_access_op(line)) {
        if (line[0] == 'r') {
            token_type = REG_ACCESS_OPERATOR;

        } else if (line[0] == '$') {
            token_type = MEM_ACCESS_OPERATOR;
        }

    } else if (is_line_number(line)) {
        token_type = NUMBER;
    }

    if (token_type == NUMBER) {
        value = str_to_num(line);
    }

    Token* token = create_token(token_type, value);

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: Create Token: type: %d value: %u\n", token_type, value);
    }

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
        return 1;
    }

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
            fprintf(stderr, "MemAlloc Error: append_to_node: token_count: %d\n", token_count);
            m_destroy();
            exit(ERROR);
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
            fprintf(stderr, "MemAlloc Error: append_to_node: token_count: %d\n", token_count);
            m_destroy();
            exit(ERROR);
        }
    }

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: Append token: token: %d | count: %d\n", token->type, token_count);
    }
    
    return 0;
}

void fix_new_line(char* line, size_t line_size) {
    size_t len = strlen(line);
    if (line[len - 1] != '\n') {
        line[len] = '\n';
    }

    if (len + 1 < line_size) {
        line[len + 1] = '\0';
    }
}

Node* tokenize_line(char* line) {
    size_t len = strlen(line);
    Node* ast_node = create_node();

    int read_flag = 1;
    char* line_buffer = line;
    int token_count = 0; //0-kw 1-op1 2-op2

    while (read_flag) {
        long unsigned int ind = 0;
        int word_start = ind;

        if (line_buffer[ind] == '\0' || line_buffer[ind] == '\n') {
            read_flag = 0;
            continue;
        }

        while (line_buffer[ind] != ' ' && !(line_buffer[ind] == '\0' || line_buffer[ind] == '\n')) {
            ind++;
        }

        int word_end = ind;
        char save = line_buffer[ind];
        line_buffer[ind] = '\0';

        if (DEBUG) {
            fprintf(stderr, "\t[DEBUG]: word: '%s'\n", line_buffer);
        }

        Token* word_token = get_token(line_buffer);

        if (is_null(word_token) || word_token->type == INVALID) {
            fprintf(stderr, "Syntax or MemAlloc Error in line: \n> '%s'", line);
            m_destroy();
            exit(ERROR);
        }


        if (append_to_node(ast_node, word_token, token_count)) {
            fprintf(stderr, "Syntax Error in line: \n> '%s'", line);
            m_destroy();
            exit(ERROR);
        }

        if (word_token->type == MEM_ACCESS_OPERATOR || word_token->type == REG_ACCESS_OPERATOR) {
            Token* num_token = get_token(line_buffer + 1);

            if (is_null(num_token) || num_token->type == INVALID) {
                fprintf(stderr, "Syntax or MemAlloc Error in line: \n> '%s'", line);
                m_destroy();
                exit(ERROR);
            }

            if (append_to_node(ast_node, num_token, token_count)) {
                fprintf(stderr, "Syntax Error in line: \n> '%s'", line);
                m_destroy();
                exit(ERROR);
            }
        }

        line_buffer[ind] = save;
        ind++;
        line_buffer += ind;
        token_count++;
    }

    return ast_node;
}

void print_node(Node* node) {
    Node* copy = node;
    printf("Head token: %d\n", copy->token->type);
    printf("Left: ");

    while (copy != NULL) {
        printf("%d ", copy->token->type);
        copy = copy->left;
    }

    printf("\n");
    copy = node;
    printf("Right: ");

    while (copy != NULL) {
        printf("%d ", copy->token->type);
        copy = copy->right;
    }

    printf("\n");
}

int main(const int argc, const char** argv) {
    if (argc < 2) {
        usage(argv[0]);
        exit(WRONG_ARGS);
    }

    const char* filename = argv[1];
    FILE* fd = fopen(filename, "r");

    if (is_null(fd)) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        perror("With Error: ");
        exit(FILE_ERROR);
    }

    char* line = NULL;
    size_t line_size = 0;

    while (!feof(fd)) {
        ssize_t readed = getline(&line, &line_size, fd);

        if (readed == -1) {
            continue;
        }

        fix_new_line(line, line_size);
        Node* tokens_line = tokenize_line(line);

        if (DEBUG) {
            print_node(tokens_line);
        }

        //get tree for one line
        //append to array with tree's

        //if (DEBUG) {
            //fprintf(stderr, "\t[DEBUG]: read line: '%s'", line);
        //}
        memset(line, 0, line_size);
    }


    //tokenize line
    //append to token array
    //parse every AST in array
    //write bytecode into out file

    if (!is_null(fd)) {
        fclose(fd);
    }

    if (!is_null(line)) {
        free(line);
    }

    m_destroy();

    return OK;
}
