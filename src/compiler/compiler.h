#ifndef __COMPILER_H__
#define __COMPILER_H__

#define _GNU_SOURCE
#define __INSTRUCTIONS_PARSE_H__ // turn off including funcs

#include <stdlib.h>
#include <string.h>

#include "../config.h" 
#include "../common/funcs.h"
#include "../instructions.h" 
#include "../str_funcs/str_funcs.h"
#include "../byte_array/byte_array.h" 
#include "../tree_translator/tree_translator.h"
#include "ast.h"
#include "keywords_array.h"
#include "../error_codes.h"

#define REG_ACCESS_WORD 'r'
#define MEM_ACCESS_WORD '$'
#define COMMENT_SYMBOL ';'

#endif

