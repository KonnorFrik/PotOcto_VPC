#include <string.h>

#include "tree_translator.h"
#include "../keywords_defines.h"
// #include "../../str_funcs/str_funcs.h"
#include "../../common/error_codes.h"
#include "../lexer/lexer.h"

#define TRN_DEBUG 1

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
    dword result = head->next->next->token->value;
    return result;
}

static dword get_lnum_operand(AST* head) {
    dword result = head->next->token->value;
    return result;
}

static void write_byte(byte* arr, size_t* ind, byte val) {
    arr[(*ind)++] = val;
}

static int translate_rsh(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xfb;

    if ( !head || !head->next || head->next->token->type == UNKNOWN || head->next->next->token->type == UNKNOWN ) {
        return ERROR;
    }

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_MEMORY) {
        code = 0xf5;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: rsh\n");
        result = ERROR;
    }

    return result;
}

static int translate_lsh(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xfa;

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xf4;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: lsh\n");
        result = ERROR;
    }

    return result;
}

static int translate_inv(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xf9;

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xf3;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: inv\n");
        result = 1;
    }

    return result;
}

static int translate_xor(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xf8;

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xf2;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: xor\n");
        result = 1;
    }

    return result;
}

static int translate_or(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xf7;

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xf1;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: or\n");
        result = 1;
    }

    return result;
}

static int translate_and(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xf6;

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xf0;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: and\n");
        result = 1;
    }

    return result;
}

static int translate_mod(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe9; // with num operand

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xe4;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: mod\n");
        result = 1;
    }

    return result;
}

static int translate_div(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe8; // with num operand

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xe3;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: div\n");
        result = 1;
    }

    return result;
}

static int translate_mul(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe7; // with num operand

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xe2;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: mul\n");
        result = 1;
    }

    return result;
}

static int translate_sub(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe6; // with num operand

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xe1;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: sub\n");
        result = 1;
    }

    return result;
}

static int translate_add(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xe5; // with num operand

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0xe0;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    //} else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        //write_byte(result_arr, result_index, code);
        //write_byte(result_arr, result_index, get_rnum_operand(head));
        //write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: add\n");
        result = 1;
    }

    return result;
}

static int translate_lt(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x2b;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate: lt\n");
        return 1;
    }

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0x25;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: lt\n");
        result = 1;
    }

    return result;
}

static int translate_gt(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x2a;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate: gt\n");
        return 1;
    }

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0x24;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: gt\n");
        result = 1;
    }

    return result;
}

static int translate_le(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x29;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate: le\n");
        return 1;
    }

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0x23;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: le\n");
        result = 1;
    }

    return result;
}

static int translate_ge(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x28;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate: ge\n");
        return 1;
    }

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0x22;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: ge\n");
        result = 1;
    }

    return result;
}

static int translate_ne(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x27;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate: ne\n");
        return 1;
    }

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0x21;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: ne\n");
        result = 1;
    }

    return result;
}

static int translate_eq(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x26;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate: eq\n");
        return 1;
    }

    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
        code = 0x20;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_lnum_operand(head));
        write_byte(result_arr, result_index, get_rnum_operand(head));

    } else if (head->next->token->type == NUMBER && head->next->next->token->type == AO_REGISTER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "UNKNOWN in translate: eq\n");
        result = 1;
    }

    return result;
}

static int translate_set(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate\n");
        return 1;
    }

    if (head->next->token->type == NUMBER) {
        byte code = 0x10;
        write_byte(result_arr, result_index, code);
        dword addr = get_lnum_operand(head);
        write_byte(result_arr, result_index, ((addr & 0xff00) >> 8));
        write_byte(result_arr, result_index, (addr & 0x00ff));

    } else {
        fprintf(stderr, "Translate: UNKNOWN tree: set\n");
        result = 1;
    }

    return result;
}

static int translate_dec(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate\n");
        return 1;
    }

    if (head->next == NULL || head->next->token->type == COMMENT ) { // inc MP
        code = 0x14;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, 0);

    } else if (head->next->token->type == AO_REGISTER/*  && head->next->next->token->type == NUMBER */) { // dec rXX
        code = 0x34;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, (word)get_lnum_operand(head));

    } else {
        fprintf(stderr, "Translate: UNKNOWN tree: dec\n");
        result = 1;
    }

    return result;
}

static int translate_inc(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate\n");
        return 1;
    }

    if (head->next == NULL || head->next->token->type == COMMENT ) { // inc MP
        code = 0x13;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, 0);

    } else if (head->next->token->type == AO_REGISTER /* && head->next->next->token->type == NUMBER */) { // inc rXX
        code = 0x33;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, (word)get_lnum_operand(head));

    } else {
        fprintf(stderr, "Translate: UNKNOWN tree: inc\n");
        result = 1;
    }

    return result;
}

static int translate_write(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    //byte code = 0x12;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate\n");
        return ERROR;
    }

    if (head->next->token->type == AO_REGISTER) {
        byte code = 0x12;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "Transate: UNKNOWN tree: write\n");
        result = ERROR;
    }

    return result;
}

static int translate_read(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    //byte code = 0x11;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate\n");
        return 1;
    }

    if (head->next->token->type == AO_REGISTER /* && head->next->next->token->type == NUMBER */) {
        byte code = 0x11;
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "Transate: UNKNOWN tree: read\n");
        result = 1;
    }

    return result;
}

static int translate_hlt(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0xff;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate\n");
        return 1;
    }

    write_byte(result_arr, result_index, code);
    write_byte(result_arr, result_index, code);
    write_byte(result_arr, result_index, code);

    return result;
}

static int translate_jmp(AST* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    // 30
    //byte code = 0;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate\n");
        return 1;
    }

    if ( head->next->token->type == NUMBER || head->next->token->type == LABEL_END ) {
        byte code = 0x30;
        dword addr = get_lnum_operand(head);

        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, (word)((addr & 0xff00) >> 8));
        write_byte(result_arr, result_index, (word)(addr & 0x00ff));

    } else {
        fprintf(stderr, "Transate: UNKNOWN tree: jmp\n");
        result = 1;
    }

    return result;
}

static int translate_mov(AST* head, byte* result_arr, size_t* result_index) {
    // 11 12 31 32
    int result = OK;
    byte code = 0;

    if (head->token->type == UNKNOWN) { // unreachable in theory
        fprintf(stderr, "UNKNOWN in translate: mov\n");
        return ERROR;
    }

    if (head->next == NULL || head->next->next == NULL) { // invalid command & bad parsing before
        fprintf(stderr, "NULL in translate: mov\n");
        return ERROR;
    }

    // mov rX 0xDD
    if (head->next->token->type == AO_REGISTER && head->next->next->token->type == NUMBER) {
        // 12 mov 0xDD rXX
        code = 0x32;

        // mov r0 r1 -> 31 1 0
    } else if (head->next->token->type == AO_REGISTER && head->next->next->token->type == AO_REGISTER) {
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

int translate_token_tree(AST* head, byte* result_arr, size_t* result_index, Options* opt) {
    if ( !head || !result_arr || !result_index ) {
        return WRONG_ARGS;
    }

    if ( (head->token->type == LABEL_START || head->token->type == LABEL_END) && head->next == NULL ) {
        return OK;
    }

    int status = OK;
    int kw_ind = 0;
    int done = 0;

    if ( opt->verbose ) {
        fprintf(stderr, "[TRANSLATOR]: Translate tree: %s\n", head->token->line);
    }

    // skip label
    while ( head->token->type == LABEL_START && head != NULL ) {
        head = head->next;
    }

    while ( !done && (status == OK) && (table[kw_ind]).kw != NULL) {
        if (strcmp(head->token->line, (table[kw_ind]).kw) == 0) {
            status = (table[kw_ind]).translate_func(head, result_arr, result_index);
            done = 1;
        }

        kw_ind++;
    }

    return status;
}
