#include "test.h"
#include "../../compiler/lexer/lexer.h"

#define MODULE "Lexer"
#define SUB_MOD "Get Token"

#define NAME "\033[38;5;46m" MODULE "\033[0m \033[38;5;45m" SUB_MOD "\033[0m"
#define TCASE_NAME "get_token"

START_TEST(test_token_get_1) {
    char* line = "mov";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, KEYWORD);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 0);

    token_destroy(obj);
}
END_TEST

START_TEST(test_token_get_2) {
    char* line = "r123";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, AO_REGISTER);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 123);

    token_destroy(obj);
}
END_TEST

START_TEST(test_token_get_3) {
    char* line = "$0x01";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, AO_MEMORY);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 1);

    token_destroy(obj);
}
END_TEST

START_TEST(test_token_get_4) {
    char* line = "0x0f";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, NUMBER);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 15);

    token_destroy(obj);
}
END_TEST

START_TEST(test_token_get_5) {
    char* line = ";0x0f";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, COMMENT);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 0);

    token_destroy(obj);
}
END_TEST

START_TEST(test_token_get_6) {
    char* line = "0b0001";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, NUMBER);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 1);

    token_destroy(obj);
}
END_TEST

START_TEST(test_token_get_7) {
    char* line = "0b0021";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, UNKNOWN);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 0);

    token_destroy(obj);
}
END_TEST

START_TEST(test_token_get_8) {
    char* line = "0x00z1";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, UNKNOWN);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 0);

    token_destroy(obj);
}
END_TEST

START_TEST(test_token_get_9) {
    char* line = "2z";

    Token* obj = token_get(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->type, UNKNOWN);
    ck_assert_str_eq(obj->line, line);
    ck_assert_uint_eq(obj->value, 0);

    token_destroy(obj);
}
END_TEST

Suite* lexer_get_token() {
    Suite* s = suite_create(NAME);
    TCase* t = tcase_create(TCASE_NAME);

    // check detecting all keywords
    tcase_add_test(t, test_token_get_1);
    tcase_add_test(t, test_token_get_2);
    tcase_add_test(t, test_token_get_3);
    tcase_add_test(t, test_token_get_4);
    tcase_add_test(t, test_token_get_5);
    tcase_add_test(t, test_token_get_6);
    tcase_add_test(t, test_token_get_7);
    tcase_add_test(t, test_token_get_8);
    tcase_add_test(t, test_token_get_9);

    suite_add_tcase(s, t);
    return s;
}

