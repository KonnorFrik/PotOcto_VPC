#ifndef __COMPILER_H__
#define __COMPILER_H__

#define _GNU_SOURCE
#define __INSTRUCTIONS_PARSE_H__ // turn off including funcs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../instructions.h" 
#include "../config.h" 
#include "../mem_manager/mem_manager.h" 
#include "../str_funcs/str_funcs.h"

enum Token_Type {
    KEYWORD = 1,
    MEM_ACCESS_OPERATOR,
    REG_ACCESS_OPERATOR,
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
