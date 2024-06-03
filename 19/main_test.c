#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_input_parse) {
    Workflows *w = NULL;
    Parts *p = NULL;
    input_parse("input_test.txt", &w, &p);
    
    mu_check(w->len == 11);
    mu_check(p->len == 5);

    workflows_free(w);
    parts_free(p);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_input_parse);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
