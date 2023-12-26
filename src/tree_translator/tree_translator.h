#ifndef __TREE_VALIDATOR_H__
#define __TREE_VALIDATOR_H__

#include <stdio.h>
#include <string.h>
#include "../config.h"
#include "../compiler/ast.h"
#include "../compiler/keywords.h"

#define TRN_DEBUG 1

typedef struct {
    char* kw;
    int (*translate_func)(Node*, byte*, size_t*);
} Table;

int translate_token_tree(Node* head, byte* result_arr, size_t* result_index);

#endif
