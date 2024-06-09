/** @file All keyword defines here */

#ifndef __KEYWORDS_DEFINES__
#define __KEYWORDS_DEFINES__

// after refactor only!!!!
// TODO: add push/pop instructions
// TODO: add call/ret instructions
// TODO: make labels system (preprocessing)

#define KW_set "set"
#define KW_read "read"
#define KW_write "write"
#define KW_mov "mov"
#define KW_jmp "jmp"
#define KW_inc "inc"
#define KW_dec "dec"
#define KW_eq "eq"
#define KW_ne "ne"
#define KW_ge "ge"
#define KW_le "le"
#define KW_gt "gt"
#define KW_lt "lt"
#define KW_add "add"
#define KW_sub "sub"
#define KW_mul "mul"
#define KW_div "div"
#define KW_mod "mod"
#define KW_and "and"
#define KW_or "or"
#define KW_xor "xor"
#define KW_lsh "lsh"
#define KW_rsh "rsh"
#define KW_inv "inv"
#define KW_hlt "hlt"

#define WORD_LABEL ':'
#define WORD_COMMENT ';'
#define AO_WORD_REGISTER 'r'
#define AO_WORD_MEMORY '$'

#endif /* __KEYWORDS_DEFINES__ */
