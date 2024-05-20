#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_input_parse) {
    Workflows *w = NULL;
    Parts *p = NULL;
    input_parse("input_test.txt", &w, &p);
    
    mu_check(0);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_input_parse);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
