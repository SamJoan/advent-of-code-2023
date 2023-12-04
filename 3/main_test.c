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

    total = find_part_numbers(
            "617*.@...", 
            ".....@.58", 
            "..592@...");
    mu_check(total == 0);

    total = find_part_numbers(
            "123123.58", 
            "617..@...", 
            "123123@hh");
    mu_check(total == 0);

    total = find_part_numbers(
            NULL, 
            "11.$.", 
            NULL);
    mu_check(total == 0);

    total = find_part_numbers(
            NULL, 
            "100\n", 
            "200\n");
    mu_check(total == 0);
}

MU_TEST(test_find_gears) {
    int total;

    total = find_gears(NULL, "467..114..", "...*......");
    mu_check(total == 0);

    total = find_gears("467..114..", "...*......", "..35..633.");
    mu_check(total == 16345);

    total = find_gears("...*......", "..35..633.", "......#...");
    mu_check(total == 0);

    total = find_gears("..35..633.", "......#...", "617*......");
    mu_check(total == 0);

    total = find_gears("......#...", "617*......", ".....+.58.");
    mu_check(total == 0);

    total = find_gears("617*......", ".....+.58.", "..592.....");
    mu_check(total == 0);

    total = find_gears(".....+.58.", "..592.....", "......755.");
    mu_check(total == 0);

    total = find_gears("..592.....", "......755.", "...$.*....");
    mu_check(total == 0);

    total = find_gears("......755.", "...$.*....", ".664.598..");
    mu_check(total == 451490);

    total = find_gears("...$.*....", ".664.598..", NULL);
    mu_check(total == 0);
}

MU_TEST(test_find_gear_ratio) {
    long int total;
    total = find_gears(
            "467..",
            "...*.",
            "..35."
        );
    mu_check(total == 16345);

    total = find_gears(
            "467.35",
            "...*..",
            "......"
        );
    mu_check(total == 16345);

    total = find_gears(
            ".467.",
            "...*.",
            "..35."
        );
    mu_check(total == 16345);

    total = find_gears(
            "..467",
            "...*.",
            "..35."
        );
    mu_check(total == 16345);

    total = find_gears(
            "...467",
            "...*..",
            "..35.."
        );
    mu_check(total == 16345);

    total = find_gears(
            "...467",
            ".35*..",
            "......"
        );
    mu_check(total == 16345);

    total = find_gears(
            "...467",
            "...*35",
            "......"
        );
    mu_check(total == 16345);
    
}

MU_TEST(test_get_number) {
    struct Number *nb = get_number("467..", 2);
    mu_check(nb->number_value == 467);
    mu_check(nb->start_pos == 0);
    mu_check(nb->end_pos == 2);
    free(nb);

    nb = get_number(".467.", 2);
    mu_check(nb->number_value == 467);
    mu_check(nb->start_pos == 1);
    mu_check(nb->end_pos == 3);
    free(nb);

    nb = get_number("..467", 2);
    mu_check(nb->number_value == 467);
    mu_check(nb->start_pos == 2);
    mu_check(nb->end_pos == 4);
    free(nb);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_find_part_numbers);
    MU_RUN_TEST(test_find_gears);
    MU_RUN_TEST(test_find_gear_ratio);
    MU_RUN_TEST(test_get_number);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
