#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_map) {
    Map *map = map_parse("input_test.txt");
    mu_check(map->len == 5);
    mu_check(map->data[1][1] == 'S');
    mu_check(map->data[4][4] == 'F');
    
    map_free(map);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");

}

MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_map);*/
    MU_RUN_TEST(test_solve_part_1);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
