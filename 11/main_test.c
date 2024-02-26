#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_galaxy) {
    Galaxy *galaxy = galaxy_parse("input_test.txt");
    mu_check(galaxy->len == 10);

    galaxy_free(galaxy);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt");

    mu_check(result == 374);
}

MU_TEST(test_hashmap) {
    CoordPairs *cp = coord_pairs_init();          
    Coord *coord_a = coord_init(140, 140);
    Coord *coord_b = coord_init(139, 139);

    coord_pairs_add(cp, coord_a, coord_b);
    coord_pairs_add(cp, coord_a, coord_b);
    coord_pairs_add(cp, coord_a, coord_b);

    Pair *pair = cp->data[hash_cp(coord_a, coord_b)];
    int len = 0;
    while(pair != NULL) {
        mu_check(coord_eq(coord_a, pair->coord_a));
        mu_check(coord_eq(coord_b, pair->coord_b));

        pair = pair->next;
        len++;
    }

    mu_check(len == 3);

    Coord *coord_c = coord_init(139, 137);
    mu_check(coord_pair_in(cp, coord_a, coord_b) == true);
    mu_check(coord_pair_in(cp, coord_b, coord_a) == true);

    mu_check(coord_pair_in(cp, coord_a, coord_c) == false);
    mu_check(coord_pair_in(cp, coord_b, coord_c) == false);

    free(coord_a);
    free(coord_b);
    free(coord_c);
    coord_pairs_free(cp);
}

MU_TEST(test_solve_part_2) {
    uint64_t result = solve_part_2("input_test.txt", 100);

    mu_check(result == 8410);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_parse_galaxy);
    MU_RUN_TEST(test_solve_part_1);
    MU_RUN_TEST(test_hashmap);
    MU_RUN_TEST(test_solve_part_2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
