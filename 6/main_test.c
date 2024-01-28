#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_paper) {
    Paper *p = paper_parse("input_test.txt");
    mu_check(p->len == 3);

    Race *r1 = p->races[0];
    mu_check(r1->time == 7);
    mu_check(r1->distance == 9);

    Race *r3 = p->races[2];
    mu_check(r3->time == 30);
    mu_check(r3->distance == 200);

    paper_free(p);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");
    mu_check(result == 288);
}
MU_TEST(test_solve_part_2) {
    uint64_t result = solve_part_1("input_test2.txt");
    mu_check(result == 71503);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_parse_paper);
    MU_RUN_TEST(test_solve_part_1);
    MU_RUN_TEST(test_solve_part_2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
