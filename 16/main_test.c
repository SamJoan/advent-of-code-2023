#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_layout) {
    Layout *l = layout_parse("input_test.txt");
    mu_check(l->len == 10);
    layout_free(l);
}

MU_TEST(test_solve_part1) {
    uint64_t result = solve_part1("input_test.txt");

    mu_check(result == 46);
}

MU_TEST(test_solve_part2) {
    uint64_t result = solve_part2("input_test.txt");

    mu_check(result == 51);
}

MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_layout);*/
    /*MU_RUN_TEST(test_solve_part1);*/
    MU_RUN_TEST(test_solve_part2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
