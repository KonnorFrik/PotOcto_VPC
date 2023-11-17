#ifndef __COMPILER_H__
#define __COMPILER_H__

#define __INSTRUCTIONS_PARSE_H__ // turn off including funcs

#include <stdio.h>
#include <stdlib.h>

#include "../instructions.h" 
#include "../config.h" 
#include "../mem_manager/mem_manager.h" 
#include "../str_funcs/str_funcs.h"

enum Type {
    KEYWORD = 1,
    ACCESS_OPERATOR,
    NUMBER,
};

typedef struct {
    int type;
    dword value;
} Token;

struct node {
    Token* token;
    struct node* left; //operand 1
    struct node* right; //operand 2
};

typedef struct node Node;

#endif
