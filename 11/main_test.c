#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_galaxy) {
    Galaxy *galaxy = parse_galaxy("input_test.txt");
    mu_check(galaxy->len == 10);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_parse_galaxy);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
