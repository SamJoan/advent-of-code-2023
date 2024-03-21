#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_hash) {
    uint64_t result = hash("HASH");
    mu_check(result == 52);
}

MU_TEST(test_parse_steps) {
    Steps *steps = parse_steps("input_test.txt");

    mu_check(steps->len == 11);
    mu_check(strcmp(steps->data[0]->label, "rn") == 0);
    mu_check(steps->data[0]->value == 1);
    mu_check(steps->data[0]->operation == '=');

    mu_check(strcmp(steps->data[1]->label, "cm") == 0);
    mu_check(steps->data[1]->operation == '-');

    mu_check(strcmp(steps->data[steps->len - 1]->label, "ot") == 0);
    mu_check(steps->data[steps->len - 1]->operation == '=');
    mu_check(steps->data[steps->len - 1]->operation == '=');

    steps_free(steps);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");
    mu_check(result == 1320);
}

MU_TEST(test_solve_part_2) {
    uint64_t result = solve_part_2("input_test.txt");
    mu_check(result == 145);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_hash);
    MU_RUN_TEST(test_parse_steps);
    MU_RUN_TEST(test_solve_part_1);
    MU_RUN_TEST(test_solve_part_2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
