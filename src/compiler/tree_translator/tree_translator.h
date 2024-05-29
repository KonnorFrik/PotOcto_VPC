#ifndef __TREE_VALIDATOR_H__
#define __TREE_VALIDATOR_H__

#include <stddef.h>
#include "../../hardware/basic_types.h"
#include "../ast.h"

/**
 * @brief Translate one token tree to byte code
 * @note This function don't care about 'result_arr' size. Write result and increase index 
 * @param[in]      head Head to valid token tree
 * @param[in, out] result_arr Array with translated bytes from ByteArray
 * @param[in, out] result_index Index from ByteArray object
 * @return status Status code from 'enum ERROR_CODES'
 */
int translate_token_tree(Node* head, byte* result_arr, size_t* result_index);

#endif /* __TREE_VALIDATOR_H__ */
