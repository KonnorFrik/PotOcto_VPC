#ifndef __TREE_VALIDATOR_H__
#define __TREE_VALIDATOR_H__

#include <stddef.h>
#include "../config.h"
#include "../compiler/ast.h"

int translate_token_tree(Node* head, byte* result_arr, size_t* result_index);

#endif
