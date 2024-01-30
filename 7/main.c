#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void determine_combos(Hand *hand, int *highest, int *second_highest, bool jokers) {
    int arr_size = 256;
    int count[arr_size];

    for(int i = 0; i < arr_size; i++) {
        count[i] = 0;
    }

    for(int i = 0; i < strlen(hand->cards); i++) {
        char c = hand->cards[i];
        count[c]++;
    }

    int nb_jokers = 0;
    if(jokers) {
        nb_jokers = count['J'];
        if(nb_jokers == 5) {
            *highest = 5;
            *second_highest = 0;
            return;
        } else if(nb_jokers > 0)  {
            count['J'] = 0;
        }
    }

    for(int i = 0; i < arr_size; i++) {
        if(count[i] != 0) {
            if(count[i] > *highest) {
                *second_highest = *highest;
                *highest = count[i];
            } else if(count[i] > *second_highest) {
                *second_highest = count[i];
            }
        }
    }

    if(jokers) {
        *highest += nb_jokers;
    }
}

enum HandType hand_determine_type(Hand *hand, bool jokers) {
    int highest = 0;
    int second_highest = 0;
    determine_combos(hand, &highest, &second_highest, jokers);

    if(highest == 5) {
        return FIVE;
    } else if(highest == 4) {
        return FOUR;
    } else if(highest == 3) {
        if(second_highest == 2) {
            return FULL;
        } else {
            return THREE;
        }
    } else if(highest == 2) {
        if(second_highest == 2) {
            return TWO_PAIR;
        } else {
            return PAIR;
        }
    } else if(highest == 1) {
        return HIGH;
    } else {
        printf("Invalid card counts. Highest %d.\n", highest);
        exit(1);
    }

    return 0;
}

Hand *hand_init(char *cards, uint64_t bid, bool jokers) {
    Hand *hand = malloc(sizeof(Hand));

    hand->bid = bid;
    hand->cards = strdup(cards);
    hand->type = hand_determine_type(hand, jokers);

    return hand;
}

Hands *hands_init() {
    Hands *hands = malloc(sizeof(Hands));
    hands->data = NULL;
    hands->len = 0;

    return hands;
}

void hands_free(Hands *hands) {
    for(int i = 0; i < hands->len; i++) {
        free(hands->data[i]->cards);
        free(hands->data[i]);
    }

    free(hands->data);
    free(hands);
}

void hands_add(Hands *hands, Hand *hand) {
    hands->len++;
    hands->data = realloc(hands->data, hands->len * sizeof(Hand*));
    hands->data[hands->len - 1] = hand;
}

Hands *parse_hands(const char *filename, bool jokers) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

        char *token;
        Hands *hands = hands_init();
	while ((read = getline(&line, &len, fp)) != -1) {
            char *line_dup = strdup(line);
            char *line_dup_orig = line_dup;
            int i = 0;
            char *cards_str = NULL;
            uint64_t bid = 0;
            while((token = strtok_r(line_dup, " ", &line_dup))) {
                if(i == 0) {
                    cards_str = strdup(token);
                } else if(i == 1) {
                    sscanf(token, "%lu", &bid);
                    Hand *hand = hand_init(cards_str, bid, jokers);
                    free(cards_str);
                    hands_add(hands, hand);
                } else {
                    printf("Issue splitting line\n");
                    exit(1);
                }

                i++;
            }

            free(line_dup_orig);
	}

	fclose(fp);
	if (line)
	    free(line);

        return hands;
}

int individual_card_compare(char *cards_a, char *cards_b, bool jokers) {
    int arr_size = 256;
    int rep[arr_size];

    for(int i = 0; i < arr_size; i++) {
        rep[i] = i;
    }

    rep['2'] = 2;
    rep['3'] = 3;
    rep['4'] = 4;
    rep['5'] = 5;
    rep['6'] = 6;
    rep['7'] = 7;
    rep['8'] = 8;
    rep['9'] = 9;
    rep['T'] = 10;
    if(jokers) {
        rep['J'] = 1;
    } else {
        rep['J'] = 11;
    }
    rep['Q'] = 12;
    rep['K'] = 13;
    rep['A'] = 14;

    assert(strlen(cards_a) == strlen(cards_b));

    for(int i = 0; i < strlen(cards_a); i++) {
        char c = cards_a[i];
        char c2 = cards_b[i];
        int card_cmp = rep[c] - rep[c2];
        if(card_cmp != 0) {
            return card_cmp;
        }
    }

    return 0;
}

int hand_compare(const void *hand_a_in, const void *hand_b_in, void *jokers_in) {
    Hand *hand_a = *(Hand **) hand_a_in;
    Hand *hand_b = *(Hand **) hand_b_in;

    bool jokers = *(bool *) jokers_in;

    int type_compare = hand_a->type - hand_b->type;

    if(type_compare == 0) {
        return individual_card_compare(hand_a->cards, hand_b->cards, jokers);
    }

    return type_compare;
}

void hands_sort(Hands *hands, bool jokers) {
    qsort_r(hands->data, hands->len, sizeof(Hand*), hand_compare, &jokers);
}

uint64_t solve_part_1(const char *filename, bool jokers) {
    Hands *hands = parse_hands(filename, jokers);

    hands_sort(hands, jokers);

    uint64_t result = 0;
    for(uint64_t i = 0; i < hands->len; i++) {
        Hand *hand = hands->data[i];
        uint64_t rank = i + 1;
        result = result + (hand->bid * rank);
    }

    hands_free(hands);

    return result;
}

uint64_t solve_part_2(char *filename) {
    return solve_part_1(filename, true);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        /*uint64_t result = solve_part_1("input.txt", false);*/
        uint64_t result = solve_part_2("input.txt");
        printf("%lu\n", result);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
