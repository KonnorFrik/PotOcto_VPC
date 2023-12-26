#ifndef __COMPILER_H__
#define __COMPILER_H__

#define _GNU_SOURCE
#define __INSTRUCTIONS_PARSE_H__ // turn off including funcs
#define CMP_DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h" 
#include "../instructions.h" 
#include "../str_funcs/str_funcs.h"
#include "../mem_manager/mem_manager.h" 
#include "../tree_translator/tree_translator.h"
#include "keywords_array.h"
#include "ast.h"

#define REG_ACCESS_WORD 'r'
#define MEM_ACCESS_WORD '$'
#define COMMENT_SYMBOL ';'

enum Local_Errors {
    SYNTAX = 1 << 10,
    INVALID_LINE = 1 << 11,
    INVALID_WORD = 1 << 12,
    TRANSLATE_LINE = 1 << 13,
};

#endif

