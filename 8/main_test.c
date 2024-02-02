#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_network_parse) {
    Network *n = network_parse("input_test.txt");
    mu_check(strcmp(n->instructions, "LLR") == 0);

    network_free(n);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");
    mu_check(result == 6);
}

MU_TEST(test_lcm) {
    size_t m_len = 5;
    uint64_t * m_arr = malloc(sizeof(uint64_t) * 5);
    m_arr[0] = 2;
    m_arr[1] = 7;
    m_arr[2] = 3;
    m_arr[3] = 9;
    m_arr[4] = 4;

    uint64_t l = lcm(m_arr, m_len);

    mu_check(l == 252);
    free(m_arr);
}

MU_TEST(test_solve_part2) {
    uint64_t result = solve_part_2("input_test.part2.txt");
    mu_check(result == 6);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_network_parse);
    MU_RUN_TEST(test_solve_part_1);
    MU_RUN_TEST(test_lcm);
    MU_RUN_TEST(test_solve_part2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
