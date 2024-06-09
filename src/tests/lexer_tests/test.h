#ifndef __LEXER_TEST__
#define __LEXER_TEST__

#include <check.h>

Suite* lexer_create_destroy();
Suite* lexer_get_token();
Suite* lexer_tokenize_line_test();

#endif /* __LEXER_TEST__ */
