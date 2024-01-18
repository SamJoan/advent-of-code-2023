#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_winning_numbers) {
    char *card_str = "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53\n";
    Card *card = malloc(sizeof(Card));

    parse_card(card, card_str);

    mu_check(card->id == 1);
    mu_check(card->winning_numbers_size == 5);
    mu_check(card->winning_numbers[0] == 41);
    mu_check(card->winning_numbers[4] == 17);

    mu_check(card->numbers_size == 8);
    mu_check(card->numbers[0] == 83);
    mu_check(card->numbers[7] == 53);

    free_card(card);
}

MU_TEST(test_calculate_score) {
    char *card_str = "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53\n";
    Card *card = malloc(sizeof(Card));
    parse_card(card, card_str);

    mu_check(calculate_score(card) == 8);

    free_card(card);
}

MU_TEST(test_sample_file) {
    int result = solve_part_1("input_test.txt");
    mu_check(result == 13);
}

MU_TEST(test_sample_file_2) {
    int result = solve_part_2("input_test.txt");
    mu_check(result == 30);
}

MU_TEST_SUITE(test_suite) {
    /*MU_RUN_TEST(test_parse_winning_numbers);*/
    /*MU_RUN_TEST(test_calculate_score);*/
    /*MU_RUN_TEST(test_sample_file);*/
    MU_RUN_TEST(test_sample_file_2);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
