#include "test.h"
#include "../../compiler/lexer/lexer.h"

#define MODULE "Lexer"
#define SUB_MOD "Create & Destroy"

#define NAME "\033[38;5;46m" MODULE "\033[0m \033[38;5;45m" SUB_MOD "\033[0m"
#define TCASE_NAME "create_destroy"

START_TEST(test_token_create_1) {
    char* kw = "mov";
    token_type type = KEYWORD;
    dword val = 0;

    Token* obj = token_create(type, kw, val);

    ck_assert_ptr_nonnull(obj);
    ck_assert_int_eq(obj->type, type);
    ck_assert_str_eq(obj->line, kw);
    ck_assert_uint_eq(obj->value, val);

    token_destroy(obj);
}

END_TEST

Suite* lexer_create_destroy() {
    Suite* s = suite_create(NAME);
    TCase* t = tcase_create(TCASE_NAME);

    tcase_add_test(t, test_token_create_1);

    suite_add_tcase(s, t);
    return s;
}
