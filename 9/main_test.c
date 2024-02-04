#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_history) {
    Report *report = parse_report("input_test.txt");

    mu_check(report->len == 3);
    mu_check(report->histories[0]->val[0] == 0);
    mu_check(report->histories[0]->val[5] == 15);
    mu_check(report->histories[2]->val[0] == 10);
    mu_check(report->histories[2]->val[5] == 45);

    report_free(report);
}

MU_TEST(test_solve_part1) {
    uint64_t result = solve_part1("input_test.txt");
    mu_check(result == 114);
}


MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_history);*/
    MU_RUN_TEST(test_solve_part1);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
