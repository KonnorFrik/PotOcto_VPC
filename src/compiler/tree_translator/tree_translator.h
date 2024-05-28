#ifndef __TREE_VALIDATOR_H__
#define __TREE_VALIDATOR_H__

#include <stddef.h>
#include "../../hardware/config.h"
#include "../ast.h"

int translate_token_tree(Node* head, byte* result_arr, size_t* result_index);

#endif
