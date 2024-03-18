#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_platform) {
    Platform *platform = parse_platform("input_test.txt");

    mu_check(platform->len == 10);
    mu_check(strcmp(platform->data[0], "O....#....") == 0);
    mu_check(strcmp(platform->data[9], "#OO..#....") == 0);

    platform_free(platform);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");
    mu_check(result == 136);
}

MU_TEST(test_solve_part_2) {
    uint64_t result = solve_part_2("input_test.txt");
    mu_check(result == 64);
}

MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_platform);*/
    /*MU_RUN_TEST(test_solve_part_1);*/
    MU_RUN_TEST(test_solve_part_2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
