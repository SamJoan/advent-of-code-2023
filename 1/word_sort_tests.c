#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "word_sort.h"

MU_TEST(test_whatever) {
    mu_check(1);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_whatever);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
