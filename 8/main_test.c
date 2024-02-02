#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_network_parse) {
    Network *n = network_parse("input_test.txt");
    printf("'%s'\n", n->instructions);
    mu_check(strcmp(n->instructions, "LLR") == 0);
    mu_check(strcmp(n->starting_node->label, "AAA") == 0);
    mu_check(strcmp(n->starting_node->left->label, "BBB") == 0);
    mu_check(strcmp(n->starting_node->right->label, "BBB") == 0);

    network_free(n);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");
    mu_check(result == 6);
}

MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_network_parse);*/
    MU_RUN_TEST(test_solve_part_1);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
