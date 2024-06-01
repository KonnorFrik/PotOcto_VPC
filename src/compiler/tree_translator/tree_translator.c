#include <string.h>

#include "tree_translator.h"
#include "../keywords_defines.h"
#include "../../str_funcs/str_funcs.h"
#include "../../common/error_codes.h"
#include "../lexer/ast.h"

#define TRN_DEBUG 0

#include <stdio.h>

typedef struct {
    char* kw;
    int (*translate_func)(AST*, byte*, size_t*);
} Table;


static int translate_mov(AST* head, byte* result_arr, size_t* result_index);
static int translate_jmp(AST* head, byte* result_arr, size_t* result_index);
static int translate_hlt(AST* head, byte* result_arr, size_t* result_index);
static int translate_set(AST* head, byte* result_arr, size_t* result_index);
static int translate_read(AST* head, byte* result_arr, size_t* result_index);
static int translate_write(AST* head, byte* result_arr, size_t* result_index);
static int translate_inc(AST* head, byte* result_arr, size_t* result_index);
static int translate_dec(AST* head, byte* result_arr, size_t* result_index);
static int translate_eq(AST* head, byte* result_arr, size_t* result_index);
static int translate_ne(AST* head, byte* result_arr, size_t* result_index);
static int translate_ge(AST* head, byte* result_arr, size_t* result_index);
static int translate_le(AST* head, byte* result_arr, size_t* result_index);
static int translate_gt(AST* head, byte* result_arr, size_t* result_index);
static int translate_lt(AST* head, byte* result_arr, size_t* result_index);
static int translate_add(AST* head, byte* result_arr, size_t* result_index);
static int translate_sub(AST* head, byte* result_arr, size_t* result_index);
static int translate_mul(AST* head, byte* result_arr, size_t* result_index);
static int translate_div(AST* head, byte* result_arr, size_t* result_index);
static int translate_mod(AST* head, byte* result_arr, size_t* result_index);

static int translate_and(AST* head, byte* result_arr, size_t* result_index);
static int translate_or(AST* head, byte* result_arr, size_t* result_index);
static int translate_xor(AST* head, byte* result_arr, size_t* result_index);
static int translate_inv(AST* head, byte* result_arr, size_t* result_index);
static int translate_lsh(AST* head, byte* result_arr, size_t* result_index);
static int translate_rsh(AST* head, byte* result_arr, size_t* result_index);

const Table table[] = {
    {KW_set, translate_set},
    {KW_inc, translate_inc},
    {KW_dec, translate_dec},
    {KW_read, translate_read},
    {KW_write, translate_write},

    {KW_eq, translate_eq},
    {KW_ne, translate_ne},
    {KW_ge, translate_ge},
    {KW_le, translate_le},
    {KW_gt, translate_gt},
    {KW_lt, translate_lt},

    {KW_add, translate_add},
    {KW_sub, translate_sub},
    {KW_mul, translate_mul},
    {KW_div, translate_div},
    {KW_mod, translate_mod},

    {KW_and, translate_and},
    {KW_or, translate_or},
    {KW_xor, translate_xor},
    {KW_inv, translate_inv},
    {KW_lsh, translate_lsh},
    {KW_rsh, translate_rsh},

    {KW_jmp, translate_jmp},
    {KW_mov, translate_mov},

    {KW_hlt, translate_hlt},
    {NULL, NULL},
};

static dword get_rnum_operand(AST* head) {
    AST* res = head;

    while (res != NULL && res->token->type != NUMBER) {
        res = res->right;
    }

    return res->token->value;
}

static dword get_lnum_operand(AST* head) {
    AST* res = head;

    while (res != NULL && res->token->type != NUMBER) {
        res = res->left;
    }

    return res->token->value;
}

static void write_byte(byte* arr, size_t* ind, byte val) {
    arr[(*ind)++] = val;
}

static int translate_rsh(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xfb;

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xf5;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: lsh\n");
        result = 1;
    }

    return result;
}

static int translate_lsh(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xfa;

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xf4;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: lsh\n");
        result = 1;
    }

    return result;
}

static int translate_inv(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xf9;

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xf3;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: inv\n");
        result = 1;
    }

    return result;
}

static int translate_xor(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xf8;

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xf2;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: xor\n");
        result = 1;
    }

    return result;
}

static int translate_or(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xf7;

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xf1;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: or\n");
        result = 1;
    }

    return result;
}

static int translate_and(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xf6;

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xf0;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: and\n");
        result = 1;
    }

    return result;
}

static int translate_mod(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe9; // with num operand

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xe4;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: mod\n");
        result = 1;
    }

    return result;
}

static int translate_div(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe8; // with num operand

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xe3;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: div\n");
        result = 1;
    }

    return result;
}

static int translate_mul(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe7; // with num operand

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xe2;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: mul\n");
        result = 1;
    }

    return result;
}

static int translate_sub(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe6; // with num operand

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xe1;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: sub\n");
        result = 1;
    }

    return result;
}

static int translate_add(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe5; // with num operand

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0xe0;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: add\n");
        result = 1;
    }

    return result;
}

static int translate_lt(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x2b;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: lt\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0x25;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: lt\n");
        result = 1;
    }

    return result;
}

static int translate_gt(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x2a;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: gt\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0x24;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: gt\n");
        result = 1;
    }

    return result;
}

static int translate_le(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x29;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: le\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0x23;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: le\n");
        result = 1;
    }

    return result;
}

static int translate_ge(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x28;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: ge\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0x22;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: ge\n");
        result = 1;
    }

    return result;
}

static int translate_ne(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x27;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: ne\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0x21;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: ne\n");
        result = 1;
    }

    return result;
}

static int translate_eq(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x26;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: eq\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == REG_ACCESS_OPERATOR) {
        code = 0x20;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->left->token->type == NUMBER && head->right->token->type == REG_ACCESS_OPERATOR) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "INVALID in translate: eq\n");
        result = 1;
    }

    return result;
}

static int translate_set(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left->token->type == NUMBER) {
        byte code = 0x10;
        write_byte(result_arr, result_index, code);
        dword addr = get_lnum_operand(head);
        write_byte(result_arr, result_index, ((addr & 0xff00) >> 8));
        write_byte(result_arr, result_index, (addr & 0x00ff));

    } else {
        fprintf(stderr, "Translate: INVALID tree: write\n");
        result = 1;
    }

    return result;
}

static int translate_dec(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->right != NULL) {
        fprintf(stderr, "Translate: INVALID tree: dec\n");
        return 1;
    }

    if (head->left == NULL) { // dec MP
        code = 0x14;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, 0);

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->left->left->token->type == NUMBER) { // dec rXX
        code = 0x34;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, (word)get_lnum_operand(head));

    } else {
        fprintf(stderr, "Translate: INVALID tree: dec\n");
        result = 1;
    }

    return result;
}

static int translate_inc(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->right != NULL) {
        fprintf(stderr, "Translate: INVALID tree: inc\n");
        return 1;
    }

    if (head->left == NULL) { // inc MP
        code = 0x13;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, 0);

    } else if (head->left->token->type == REG_ACCESS_OPERATOR && head->left->left->token->type == NUMBER) { // inc rXX
        code = 0x33;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, (word)get_lnum_operand(head));

    } else {
        fprintf(stderr, "Translate: INVALID tree: inc\n");
        result = 1;
    }

    return result;
}

static int translate_write(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    //byte code = 0x12;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->left->left->token->type == NUMBER) {
        byte code = 0x12;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "Transate: INVALID tree: write\n");
        result = 1;
    }

    return result;
}

static int translate_read(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    //byte code = 0x11;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->left->left->token->type == NUMBER) {
        byte code = 0x11;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "Transate: INVALID tree: read\n");
        result = 1;
    }

    return result;
}

static int translate_hlt(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xff;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    write_byte(result_arr, result_index, code);
    write_byte(result_arr, result_index, code);
    write_byte(result_arr, result_index, code);

    return result;
}

static int translate_jmp(AST* head, byte* result_arr, size_t* result_index) {
    // 30
    int result = OK;
    //byte code = 0;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left->token->type == NUMBER) {
        byte code = 0x30;
        result_arr[(*result_index)++] = code;
        dword addr = get_lnum_operand(head);

        write_byte(result_arr, result_index, (word)((addr & 0xff00) >> 8));
        write_byte(result_arr, result_index, (word)(addr & 0x00ff));

    } else {
        fprintf(stderr, "Transate: INVALID tree: jmp\n");
        result = 1;
    }

    return result;
}

static int translate_mov(AST* head, byte* result_arr, size_t* result_index) {
    // 11 12 31 32
    int result = OK;
    byte code = 0;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: mov\n");
        return 1;
    }

    if (head->left == NULL || head->right == NULL) { // invalid command & bad parsing before
        fprintf(stderr, "NULL in translate: mov\n");
        return 1;
    }

    //if (head->left->token->type == MEM_ACCESS_OPERATOR && !(head->right->token->type == MEM_ACCESS_OPERATOR || head->right->token->type == NUMBER)) { // deprecated
        //// 11 mov mem[MP] rXX
        //code = 0x11;
//
    //} else if (head->right->token->type == MEM_ACCESS_OPERATOR && !(head->left->token->type == MEM_ACCESS_OPERATOR || head->left->token->type == NUMBER)) { // deprecated
        //// 12 mov rXX mem[MP]
        //code = 0x12;

        // mov 5 r0 -> 32 0 5
    if (head->left->token->type == REG_ACCESS_OPERATOR && head->right->token->type == NUMBER) {
        // 12 mov 0xDD rXX
        code = 0x32;

        // mov r0 r1 -> 31 1 0
    } else if (head->right->token->type == REG_ACCESS_OPERATOR && head->left->token->type == REG_ACCESS_OPERATOR) {
        // 12 mov rXX rYY
        code = 0x31;

    } else {
        fprintf(stderr, "Unknown Tree: translate: mov\n");
        result = 1;
    }

    if (!result) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));
    }

    return result;
}

int translate_token_tree(AST* head, byte* result_arr, size_t* result_index) {
    int status = OK;
    int kw_ind = 0;

#if TRN_DEBUG == 1
        fprintf(stderr, "\t[TRN_DEBUG]: Translate tree: %s\n", head->token->word);
#endif

    while (status == OK && (table[kw_ind]).kw != NULL) {
        if (strcmp(head->token->word, (table[kw_ind]).kw) == 0) {
            status = (table[kw_ind]).translate_func(head, result_arr, result_index);
        }

        kw_ind++;
    }

    return status;
}
