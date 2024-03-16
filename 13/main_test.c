#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_patterns) {
    Patterns *p = parse_patterns("input_test.txt");
    mu_check(p->len == 2);
    
    Pattern *first = p->data[0];
    mu_check(first->len == 7);
    mu_check(strcmp(first->data[0], "#.##..##.") == 0);
    mu_check(strcmp(first->data[first->len - 1], "#.#.##.#.") == 0);

    Pattern *second = p->data[1];
    mu_check(second->len == 7);
    mu_check(strcmp(second->data[0], "#...##..#") == 0);
    mu_check(strcmp(second->data[second->len - 1], "#....#..#") == 0);

    patterns_free(p);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");
    mu_check(result == 405);
}

MU_TEST(test_solve_part_2) {
    uint64_t result = solve_part_2("input_test.txt");
    mu_check(result == 400);
}

MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_patterns);*/
    /*MU_RUN_TEST(test_solve_part_1);*/
    MU_RUN_TEST(test_solve_part_2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
