#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_map) {
    Map *map = map_parse("input_test.txt");
    mu_check(map->len == 13);
    map_free(map);
}

MU_TEST(test_priority_queue) {
    PriorityQueue *pq = pq_init();

    Elem *e3 = elem_init(3, 3, 3, HORIZONTAL);
    Elem *e4 = elem_init(4, 4, 4, HORIZONTAL);
    Elem *e2 = elem_init(2, 2, 2, HORIZONTAL);
    Elem *e1 = elem_init(1, 1, 1, HORIZONTAL);
    Elem *e0 = elem_init(0, 0, 0, HORIZONTAL);
    Elem *e5 = elem_init(5, 5, 5, HORIZONTAL);

    pq_insert(pq, e5);
    pq_insert(pq, e4);
    pq_insert(pq, e3);
    pq_insert(pq, e2);
    pq_insert(pq, e1);
    pq_insert(pq, e0);

    Elem *min;

    min = pq_extract_min(pq);
    mu_check(min->x == 0);
    free(min);

    min = pq_extract_min(pq);
    mu_check(min->x == 1);
    free(min);

    min = pq_extract_min(pq);
    mu_check(min->x == 2);
    free(min);

    min = pq_extract_min(pq);
    mu_check(min->x == 3);
    free(min);

    min = pq_extract_min(pq);
    mu_check(min->x == 4);
    free(min);

    min = pq_extract_min(pq);
    mu_check(min->x == 5);
    free(min);

    min = pq_extract_min(pq);
    mu_check(min == NULL);
    free(min);

    pq_free(pq);
}

MU_TEST(test_solve_part1) {
    uint64_t result = solve_part1("input_test.txt");
    mu_check(result == 102);
}

MU_TEST(test_solve_part2) {
    uint64_t result = solve_part2("input_test.txt");
    mu_check(result == 94);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_parse_map);
    MU_RUN_TEST(test_priority_queue);
    MU_RUN_TEST(test_solve_part1);
    MU_RUN_TEST(test_solve_part2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
