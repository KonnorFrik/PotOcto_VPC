#include "compiler.h"

void usage(const char* prog_name) {
    printf("Usage: %s file\n", prog_name);
}

Token* create_token(int type, dword value) {
    Token* obj = calloc(1, sizeof(Token));

    if (!is_null(obj)) {
        obj->type = type;
        obj->value = value;
    }

    return obj;
}

Node* create_node() {
    Node* obj = calloc(1, sizeof(Node));

    if (!is_null(obj)) {
        obj->token = NULL;
        obj->left = NULL;
        obj->right = NULL;
    }

    return obj;
}

int main(const int argc, const char** argv) {
    if (argc < 2) {
        usage(argv[0]);
        exit(FILE_ERROR);
    }

    printf("str type: %d\n", string_type(argv[1]));

    //check file
    //read line from file
    //tokenize line
    //append to token array
    //parse every AST in array
    //write bytecode into out file

    return OK;
}
