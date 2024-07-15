#include "lib/util.h"
#include "main.h"
#include "minunit.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

MU_TEST(test_input_parse) {
    HashMap *w = NULL;
    Parts *p = NULL;
    input_parse("input_test.txt", &w, &p);
    
    int len = 0;
    for(int i = 0; i < w->max_size; i++) {
        KeyValue *kv = w->data[i];
        if(kv != NULL) {
            len++;
        }
    }
    mu_check(len == 11);
    mu_check(p->len == 5);

    parts_free(p);
    workflows_free(w);
}

MU_TEST(test_solve_part1) {
    uint64_t result = solve_part1("input_test.txt");

    mu_check(result == 19114);
}

MU_TEST(test_solve_part2) {
    uint64_t result = solve_part2("input_test.txt");

    mu_check(result == 167409079868000);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_input_parse);
    MU_RUN_TEST(test_solve_part1);
    MU_RUN_TEST(test_solve_part2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
