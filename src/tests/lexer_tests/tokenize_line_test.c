#include "test.h"
#include "../../compiler/lexer/lexer.h"

#define MODULE "Lexer"
#define SUB_MOD "Tokenize Line"

#define NAME "\033[38;5;46m" MODULE "\033[0m \033[38;5;45m" SUB_MOD "\033[0m"
#define TCASE_NAME "tokenize line"

START_TEST(test_1_token_line) {
    char* line = "mov r1 2";

    AST* obj = lexer_tokenize_line(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->token->type, KEYWORD);
    ck_assert_str_eq(obj->token->line, "mov");
    ck_assert_uint_eq(obj->token->value, 0);
    obj = obj->next;

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->token->type, AO_REGISTER);
    ck_assert_str_eq(obj->token->line, "r1");
    ck_assert_uint_eq(obj->token->value, 1);
    obj = obj->next;

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->token->type, NUMBER);
    ck_assert_str_eq(obj->token->line, "2");
    ck_assert_uint_eq(obj->token->value, 2);
    obj = obj->next;

    ck_assert_ptr_null(obj);

    ast_destroy(obj);
}
END_TEST

START_TEST(test_2_token_line) {
    char* line = ";mov r1 2";

    AST* obj = lexer_tokenize_line(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->token->type, COMMENT);
    ck_assert_str_eq(obj->token->line, ";mov");
    ck_assert_uint_eq(obj->token->value, 0);
    obj = obj->next;

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->token->type, AO_REGISTER);
    ck_assert_str_eq(obj->token->line, "r1");
    ck_assert_uint_eq(obj->token->value, 1);
    obj = obj->next;

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->token->type, NUMBER);
    ck_assert_str_eq(obj->token->line, "2");
    ck_assert_uint_eq(obj->token->value, 2);
    obj = obj->next;

    ck_assert_ptr_null(obj);

    ast_destroy(obj);
}
END_TEST

START_TEST(test_3_token_line) {
    char* line = "jmp lbl";

    AST* obj = lexer_tokenize_line(line);

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->token->type, KEYWORD);
    ck_assert_str_eq(obj->token->line, "jmp");
    ck_assert_uint_eq(obj->token->value, 0);
    obj = obj->next;

    ck_assert_ptr_nonnull(obj);
    ck_assert_uint_eq(obj->token->type, UNKNOWN);
    ck_assert_str_eq(obj->token->line, "lbl");
    ck_assert_uint_eq(obj->token->value, 0);
    obj = obj->next;

    ck_assert_ptr_null(obj);

    ast_destroy(obj);
}
END_TEST

Suite* lexer_tokenize_line_test() {
    Suite* s = suite_create(NAME);
    TCase* t = tcase_create(TCASE_NAME);

    tcase_add_test(t, test_1_token_line);
    tcase_add_test(t, test_2_token_line);
    tcase_add_test(t, test_3_token_line);

    suite_add_tcase(s, t);
    return s;
}


