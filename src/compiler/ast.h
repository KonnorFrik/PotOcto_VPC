#ifndef __AST__
#define __AST__

enum {
    KEYWORD = 1,
    MEM_ACCESS_OPERATOR, // 2
    REG_ACCESS_OPERATOR, // 3
    NUMBER,              // 4
};

typedef struct {
    int type;
    char* word;
    dword value;
} Token;

typedef struct node {
    Token* token;
    struct node* left; //operand 1
    struct node* right; //operand 2
} Node;

#endif

