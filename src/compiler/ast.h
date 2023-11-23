#ifndef __AST__
#define __AST__

enum Token_Type {
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

struct node {
    Token* token;
    struct node* left; //operand 1
    struct node* right; //operand 2
};

typedef struct node Node;

#endif

