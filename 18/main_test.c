#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_points_p1) {
    Points *points = parse_points("input_test.txt", '1');

    Point *first = points->data[0];
    Point *last = points->data[points->len - 1];

    mu_check(first->x == 0);
    mu_check(first->y == 0);

    mu_check(last->x == 0);
    mu_check(last->y == 0);

    points_free(points);
}

MU_TEST(test_parse_points_p2) {
    Points *points = parse_points("input_test.txt", '2');

    Point *first = points->data[0];
    Point *last = points->data[points->len - 1];
    Point *second = points->data[0];

    mu_check(first->x == 0);
    mu_check(first->y == 0);

    mu_check(second->x == 461937);
    mu_check(second->y == 0);

    mu_check(last->x == 0);
    mu_check(last->y == 0);

    points_free(points);
}

MU_TEST(test_solve_part1) {
    uint64_t result = solve_part1("input_test.txt");

    mu_check(result == 62);
}


MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_points_p1);*/
    /*MU_RUN_TEST(test_solve_part1);*/
    MU_RUN_TEST(test_parse_points_p2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
