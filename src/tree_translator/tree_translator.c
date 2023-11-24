#include "tree_translator.h"

static int translate_mov(Node* head, byte* result_arr, size_t* result_index);
static int translate_jmp(Node* head, byte* result_arr, size_t* result_index);
static int translate_hlt(Node* head, byte* result_arr, size_t* result_index);
static int translate_set(Node* head, byte* result_arr, size_t* result_index);
static int translate_read(Node* head, byte* result_arr, size_t* result_index);
static int translate_write(Node* head, byte* result_arr, size_t* result_index);
static int translate_inc(Node* head, byte* result_arr, size_t* result_index);
static int translate_dec(Node* head, byte* result_arr, size_t* result_index);
static int translate_eq(Node* head, byte* result_arr, size_t* result_index);
static int translate_ne(Node* head, byte* result_arr, size_t* result_index);
static int translate_ge(Node* head, byte* result_arr, size_t* result_index);
static int translate_le(Node* head, byte* result_arr, size_t* result_index);
static int translate_gt(Node* head, byte* result_arr, size_t* result_index);
static int translate_lt(Node* head, byte* result_arr, size_t* result_index);

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

    {KW_jmp, translate_jmp},
    {KW_mov, translate_mov},

    {KW_hlt, translate_hlt},
    {NULL, NULL},
};

static dword get_rnum_operand(Node* head) {
    Node* res = head;

    while (res != NULL && res->token->type != NUMBER) {
        res = res->right;
    }

    return res->token->value;
}

static dword get_lnum_operand(Node* head) {
    Node* res = head;

    while (res != NULL && res->token->type != NUMBER) {
        res = res->left;
    }

    return res->token->value;
}

static void write_byte(byte* arr, size_t* ind, byte val) {
    arr[(*ind)++] = val;
}

static int translate_lt(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x2b;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: eq\n");
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
        fprintf(stderr, "INVALID in translate: eq\n");
        result = 1;
    }

    return result;
}

static int translate_gt(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x2a;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: eq\n");
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
        fprintf(stderr, "INVALID in translate: eq\n");
        result = 1;
    }

    return result;
}

static int translate_le(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x29;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: eq\n");
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
        fprintf(stderr, "INVALID in translate: eq\n");
        result = 1;
    }

    return result;
}

static int translate_ge(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x28;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: eq\n");
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
        fprintf(stderr, "INVALID in translate: eq\n");
        result = 1;
    }

    return result;
}

static int translate_ne(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x27;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate: eq\n");
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
        fprintf(stderr, "INVALID in translate: eq\n");
        result = 1;
    }

    return result;
}

static int translate_eq(Node* head, byte* result_arr, size_t* result_index) {
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

static int translate_set(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x10;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left->token->type == NUMBER) {
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

static int translate_dec(Node* head, byte* result_arr, size_t* result_index) {
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

static int translate_inc(Node* head, byte* result_arr, size_t* result_index) {
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

static int translate_write(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x12;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->left->left->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "Transate: INVALID tree: write\n");
        result = 1;
    }

    return result;
}

static int translate_read(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    byte code = 0x11;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left->token->type == REG_ACCESS_OPERATOR && head->left->left->token->type == NUMBER) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, 0);
        write_byte(result_arr, result_index, get_lnum_operand(head));

    } else {
        fprintf(stderr, "Transate: INVALID tree: read\n");
        result = 1;
    }

    return result;
}

static int translate_hlt(Node* head, byte* result_arr, size_t* result_index) {
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

static int translate_jmp(Node* head, byte* result_arr, size_t* result_index) {
    // 30
    int result = OK;
    byte code = 0;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left->token->type == NUMBER) {
        code = 0x30;
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

static int translate_mov(Node* head, byte* result_arr, size_t* result_index) {
    // 11 12 31 32
    int result = OK;
    byte code = 0;

    if (head->token->type == INVALID) { // unreachable in theory
        fprintf(stderr, "INVALID in translate\n");
        return 1;
    }

    if (head->left == NULL || head->right == NULL) { // invalid command
        fprintf(stderr, "NULL in translate: mov\n");
        return 1;
    }

    if (head->left->token->type == MEM_ACCESS_OPERATOR && !(head->right->token->type == MEM_ACCESS_OPERATOR || head->right->token->type == NUMBER)) {
        // 11 mov mem[MP] rXX
        code = 0x11;

    } else if (head->right->token->type == MEM_ACCESS_OPERATOR && !(head->left->token->type == MEM_ACCESS_OPERATOR || head->left->token->type == NUMBER)) {
        // 12 mov rXX mem[MP]
        code = 0x12;

    } else if (head->right->token->type == REG_ACCESS_OPERATOR && !(head->left->token->type == REG_ACCESS_OPERATOR)) {
        // 12 mov 0xDD rXX
        code = 0x32;

    } else if (head->right->token->type == REG_ACCESS_OPERATOR && head->left->token->type == REG_ACCESS_OPERATOR) {
        // 12 mov rXX rYY
        code = 0x31;

    } else {
        fprintf(stderr, "Unknown Tree: translate: mov\n");
        result = 1;
    }

    if (!result) {
        write_byte(result_arr, result_index, code);
        write_byte(result_arr, result_index, get_rnum_operand(head));
        write_byte(result_arr, result_index, get_lnum_operand(head));
    }

    return result;
}

int translate_token_tree(Node* head, byte* result_arr, size_t* result_index) {
    int result = OK;
    int kw_ind = 0;

    if (TRN_DEBUG) {
        fprintf(stderr, "\t[TRN_DEBUG]: Translate tree: %s\n", head->token->word);
    }

    while (result == OK && (table[kw_ind]).kw != NULL) {
        if (strcmp(head->token->word, (table[kw_ind]).kw) == 0) {
            result = (table[kw_ind]).translate_func(head, result_arr, result_index);
        }

        kw_ind++;
    }

    return result;
}
