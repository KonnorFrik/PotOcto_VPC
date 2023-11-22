#include "tree_translator.h"

static int translate_mov(Node* head, byte* result_arr, int* result_index);
static int translate_jmp(Node* head, byte* result_arr, int* result_index);

const Table table[] = {
    {KW_mov, translate_mov},
    {KW_jmp, translate_jmp},
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

static int translate_jmp(Node* head, byte* result_arr, int* result_index) {
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

        result_arr[(*result_index)++] = (word)((addr & 0xff00) >> 8);
        result_arr[(*result_index)++] = (word)(addr & 0x00ff);

    } else {
        fprintf(stderr, "Transate: INVALID tree: jmp\n");
        result = 1;
    }

    return result;
}

static int translate_mov(Node* head, byte* result_arr, int* result_index) {
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
        result_arr[(*result_index)++] = code;
        result_arr[(*result_index)++] = get_lnum_operand(head);
        result_arr[(*result_index)++] = get_rnum_operand(head);
    }

    return result;
}

int translate_token_tree(Node* head, byte* result_arr, int* result_index) {
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
