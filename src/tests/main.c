#include <check.h>
#include <stdio.h>

#include "lexer_tests/test.h"

typedef struct {
  int all;
  int failed;
} Stat;


void run_test(Suite* obj, Stat* stat) {
    SRunner* runner = srunner_create(obj);
    srunner_run_all(runner, CK_NORMAL);

    stat->failed += srunner_ntests_failed(runner);
    stat->all += srunner_ntests_run(runner);

    srunner_free(runner);
}

int main() {
    Stat stat = {0};

    run_test(lexer_create_destroy(), &stat);
    run_test(lexer_get_token(), &stat);
    run_test(lexer_tokenize_line_test(), &stat);

    printf("\n\t\033[38;2;0;250;0m    all\033[0m: %d", stat.all);
    printf("\n\t\033[38;2;250;0;0m failed\033[0m: %d\n", stat.failed);

}
