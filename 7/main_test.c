#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_hands) {
    Hands *hands = parse_hands("input_test.txt", false);

    mu_check(hands->len == 5);

    hands_free(hands);
}

MU_TEST(test_hand_compare) {
    bool jokers = false;
    Hand *hand1 = hand_init("32T3K", 0, false);
    Hand *hand2 = hand_init("T55J5", 0, false);
    Hand *hand3 = hand_init("KK677", 0, false);
    Hand *hand4 = hand_init("KTJJT", 0, false);
    Hand *hand5 = hand_init("QQQJA", 0, false);

    mu_check(hand1->type == PAIR);
    mu_check(hand2->type == THREE);
    mu_check(hand3->type == TWO_PAIR);
    mu_check(hand4->type == TWO_PAIR);
    mu_check(hand5->type == THREE);

    mu_check(hand_compare(&hand1, &hand2, &jokers) < 0);
    mu_check(hand_compare(&hand1, &hand3, &jokers) < 0);
    mu_check(hand_compare(&hand1, &hand4, &jokers) < 0);
    mu_check(hand_compare(&hand1, &hand5, &jokers) < 0);

    mu_check(hand_compare(&hand2, &hand1, &jokers) > 0);
    mu_check(hand_compare(&hand2, &hand3, &jokers) > 0);
    mu_check(hand_compare(&hand2, &hand4, &jokers) > 0);
    mu_check(hand_compare(&hand2, &hand5, &jokers) < 0);

    mu_check(hand_compare(&hand3, &hand1, &jokers) > 0);
    mu_check(hand_compare(&hand3, &hand4, &jokers) > 0);
    mu_check(hand_compare(&hand3, &hand2, &jokers) < 0);
    mu_check(hand_compare(&hand3, &hand5, &jokers) < 0);

    free(hand1->cards);
    free(hand1);
    free(hand2->cards);
    free(hand2);
    free(hand3->cards);
    free(hand3);
    free(hand4->cards);
    free(hand4);
    free(hand5->cards);
    free(hand5);
}

MU_TEST(test_individual_card_compare) {
    mu_check(individual_card_compare("33332", "2AAAA", false) > 0);
    mu_check(individual_card_compare("77888", "77788", false) > 0);
    mu_check(individual_card_compare("77888", "77788", false) > 0);

    mu_check(individual_card_compare("JKKK2", "QQQQ2", false) < 0);
    mu_check(individual_card_compare("JKKK2", "2QQQ2", false) > 0);
    mu_check(individual_card_compare("JKKK2", "2QQQ2", true) < 0);
}

MU_TEST(test_hands_sort) {
    Hand *hand1 = hand_init("32T3K", 0, false);
    Hand *hand2 = hand_init("T55J5", 0, false);
    Hand *hand3 = hand_init("KK677", 0, false);
    Hand *hand4 = hand_init("KTJJT", 0, false);
    Hand *hand5 = hand_init("QQQJA", 0, false);

    Hands *hands = hands_init();

    hands_add(hands, hand1);
    hands_add(hands, hand2);
    hands_add(hands, hand3);
    hands_add(hands, hand4);
    hands_add(hands, hand5);

    hands_sort(hands, false);

    mu_check(strcmp(hands->data[0]->cards, "32T3K") == 0);
    mu_check(strcmp(hands->data[2]->cards, "KK677") == 0);
    mu_check(strcmp(hands->data[4]->cards, "QQQJA") == 0);

    hands_free(hands);
}

MU_TEST(test_solve_part_1) {
    uint64_t result = solve_part_1("input_test.txt", false);
    mu_check(result == 6440);
}

MU_TEST(test_hand_compare_jokers) {
    Hand *hand1 = hand_init("32T3K", 0, true);
    Hand *hand2 = hand_init("T55J5", 0, true);
    Hand *hand3 = hand_init("KK677", 0, true);
    Hand *hand4 = hand_init("KTJJT", 0, true);
    Hand *hand5 = hand_init("QQQJA", 0, true);

    mu_check(hand1->type == PAIR);
    mu_check(hand2->type == FOUR);
    mu_check(hand3->type == TWO_PAIR);
    mu_check(hand4->type == FOUR);
    mu_check(hand5->type == FOUR);

    free(hand1->cards);
    free(hand1);
    free(hand2->cards);
    free(hand2);
    free(hand3->cards);
    free(hand3);
    free(hand4->cards);
    free(hand4);
    free(hand5->cards);
    free(hand5);
}

MU_TEST(test_solve_part_2) {
    uint64_t result = solve_part_2("input_test.txt");
    mu_check(result == 5905);
}

MU_TEST(test_input_test_mine) {
    uint64_t result = solve_part_2("input_test_mine.txt");
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_parse_hands);
    MU_RUN_TEST(test_hand_compare);
    MU_RUN_TEST(test_individual_card_compare);
    MU_RUN_TEST(test_hands_sort);
    MU_RUN_TEST(test_solve_part_1);
    MU_RUN_TEST(test_hand_compare_jokers);
    MU_RUN_TEST(test_solve_part_2);
    MU_RUN_TEST(test_input_test_mine);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
