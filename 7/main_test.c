#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_hands) {
    Hands *hands = parse_hands("input_test.txt");

    mu_check(hands->len == 5);

    hands_free(hands);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");
    /*mu_check(result == 251058093);*/
}

MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_hands);*/
    MU_RUN_TEST(test_solve_part_1);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
