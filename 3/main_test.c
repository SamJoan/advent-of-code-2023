#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_find_part_numbers) {
    int total;

    total = find_part_numbers(NULL, "467..114..", "...*......");
    mu_check(total == 467);

    total = find_part_numbers("467..114..", "...*......", "..35..633.");
    mu_check(total == 0);

    total = find_part_numbers("...*......", "..35..633.", "......#...");
    mu_check(total == 668);

    total = find_part_numbers("..35..633.", "......#...", "617*......");
    mu_check(total == 0);

    total = find_part_numbers("......#...", "617*......", ".....+.58.");
    mu_check(total == 617);

    total = find_part_numbers("617*......", ".....+.58.", "..592.....");
    mu_check(total == 0);

    total = find_part_numbers(".....+.58.", "..592.....", "......755.");
    mu_check(total == 592);

    total = find_part_numbers("..592.....", "......755.", "...$.*....");
    mu_check(total == 755);

    total = find_part_numbers("......755.", "...$.*....", ".664.598..");
    mu_check(total == 0);

    total = find_part_numbers("...$.*....", ".664.598..", NULL);
    mu_check(total == 1262);

    total = find_part_numbers(
            "617*.@....@", 
            ".....@.58.@", 
            "..592@....@");
    mu_check(total == 0);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_find_part_numbers);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
