#include "main.h"
#include "minunit.h"
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

MU_TEST(test_parse_almanac) {
    Seeds *seeds = NULL;
    Maps *maps = NULL;

    parse_almanac("input_test.txt", &seeds, &maps);

    mu_check(seeds->len == 4);
    mu_check(seeds->seeds[0] == 79UL);
    mu_check(seeds->seeds[3] == 13UL);

    mu_check(maps->len == 7);
    mu_check(strcmp(maps->maps[0]->name, "seed-to-soil") == 0);
    mu_check(strcmp(maps->maps[1]->name, "soil-to-fertilizer") == 0);
    mu_check(strcmp(maps->maps[2]->name, "fertilizer-to-water") == 0);
    mu_check(strcmp(maps->maps[3]->name, "water-to-light") == 0);
    mu_check(strcmp(maps->maps[6]->name, "humidity-to-location") == 0);

    mu_check(maps->maps[0]->len == 2);
    mu_check(maps->maps[2]->len == 4);
    mu_check(maps->maps[4]->len == 3);
    mu_check(maps->maps[6]->len == 2);

    mu_check(maps->maps[0]->rules[0]->dst_start == 50);
    mu_check(maps->maps[0]->rules[0]->dst_end == 51);
    mu_check(maps->maps[0]->rules[0]->src_start == 98);
    mu_check(maps->maps[0]->rules[0]->src_end == 99);

    mu_check(maps->maps[6]->rules[1]->dst_start == 56);
    mu_check(maps->maps[6]->rules[1]->dst_end == 59);
    mu_check(maps->maps[6]->rules[1]->src_start == 93);
    mu_check(maps->maps[6]->rules[1]->src_end == 96);

    for(int i = 0; i < maps->len; i++) {
        for(int j = 0; j < maps->maps[i]->len; j++) {
            free(maps->maps[i]->rules[j]);
        }
        free(maps->maps[i]->rules);
        free(maps->maps[i]->name);
        free(maps->maps[i]);
    }
    free(maps->maps);
    free(maps);

    free(seeds->seeds);
    free(seeds);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_parse_almanac);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
