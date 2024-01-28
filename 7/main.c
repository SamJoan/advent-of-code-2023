#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Hand *hand_init(char *cards, uint64_t bid) {
    Hand *hand = malloc(sizeof(Hand));

    hand->len = strlen(cards);
    hand->bid = bid;
    hand->cards = cards;

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

Hands *parse_hands(const char *filename) {
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
                    Hand *hand = hand_init(cards_str, bid);
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

int hand_compare(const void *hand_a_in, const void *hand_b_in) {
    Hand *hand_a = *(Hand **) hand_a_in;
    Hand *hand_b = *(Hand **) hand_b_in;
    printf("%s < %s?\n", hand_a->cards, hand_b->cards);
    return -1;
}

uint64_t solve_part_1(const char *filename) {
    Hands *hands = parse_hands(filename);

    qsort(hands->data, hands->len, sizeof(Hand*), hand_compare);
    for(int i = 0; i < hands->len; i++) {
        printf("%p %s %lu\n", hands->data[i], hands->data[i]->cards, hands->data[i]->bid);
    }

    hands_free(hands);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
