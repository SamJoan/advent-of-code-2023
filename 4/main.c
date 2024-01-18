#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_ints(char *numbers_in, int **numbers_ptr) {
    char *numbers_str = strdup(numbers_in);
    char *numbers_str_orig = numbers_str;
    
    int *numbers = NULL;
    char *token = NULL;
    int i = 0;
    while((token = strtok_r(numbers_str, " ", &numbers_str))) {
        numbers = realloc(numbers, (i + 1) * sizeof(int));

        sscanf(token, "%d", &numbers[i]);

        i++;
    }

    free(numbers_str_orig);
    *numbers_ptr = numbers;

    return i;
}

void parse_numbers(char *all_numbers_str_in, Card *card) {
    char *all_numbers = strdup(all_numbers_str_in);
    char *all_numbers_orig = all_numbers;

    char *token = NULL;
    char *winning_numbers = NULL;
    char *numbers = NULL;
    int i = -1;
    while((token = strtok_r(all_numbers, "|", &all_numbers))) {
        if(winning_numbers == NULL) {
            card->winning_numbers_size = parse_ints(token, &card->winning_numbers);
            winning_numbers = token;
        } else {
            card->numbers_size = parse_ints(token, &card->numbers);
        }
    }

    free(all_numbers_orig);
}

void free_card(Card *card) {
    free(card->winning_numbers);
    free(card->numbers);
    free(card);
}

void parse_card(Card *card, const char *card_str_in) {
    char *card_str = strdup(card_str_in);
    char *card_str_orig = card_str;
    char *token = NULL;

    char *card_id_str = NULL;
    char *all_numbers_str = NULL;
    while((token = strtok_r(card_str, ":", &card_str))) {
        if(card_id_str == NULL) {
            card_id_str = token;
            sscanf(card_id_str, "Card %d", &card->id);
        } else {
            all_numbers_str = token;
            parse_numbers(all_numbers_str, card);
        }
    }

    card->copies = 1;

    free(card_str_orig);
}

bool is_winning_number(Card *card, int nb) {
    for(int i = 0; i < card->winning_numbers_size; i++) {
        if(nb == card->winning_numbers[i]) {
            return true;
        }
    }

    return false;
}

int count_valid(Card *card) {
    int valid = 0;
    for(int i = 0; i < card->numbers_size; i++) {
        int number = card->numbers[i];
        if(is_winning_number(card, number)) {
            valid += 1;
        }
        
    }

    return valid;
}

int calculate_score(Card *card) {
    int score = 0;
    int valid = count_valid(card);
    if(valid > 0) {
        score = 1;
        for(int i = 0; i < valid - 1; i++) {
            score = score * 2;
        }
    }

    return score;
}

int solve_part_1(char *filename) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can't read file.\n");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        Card *card = malloc(sizeof(Card));
        parse_card(card, line);
        result += calculate_score(card);
        free_card(card);
    }

    fclose(fp);
    if (line)
        free(line);

    return result;
}

int get_cards(const char *filename, Card ***cards_out) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can't read file.\n");
        exit(EXIT_FAILURE);
    }
    Card **cards = NULL;

    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        cards = realloc(cards, sizeof(Card*) * (i + 1));
        cards[i] = malloc(sizeof(Card));

        parse_card(cards[i], line);
        result += calculate_score(cards[i]);
        i++;
    }

    fclose(fp);
    if (line)
        free(line);

    *cards_out = cards;

    return i;
}

int solve_part_2(char *filename) {
    Card **cards = NULL;
    int nb_cards = get_cards(filename, &cards);
    for(int i = 0; i < nb_cards; i++) {
        Card *card = cards[i];
        int valid = count_valid(card);
        for(int k = 0; k < card->copies; k++) {
            for(int j = 0; j < valid; j++) {
                int index = i + j + 1;
                if(index >= nb_cards) {
                    break;
                }
                cards[index]->copies += 1;
            }
        }
    }

    int total_copies = 0;
    for(int i = 0; i < nb_cards; i++) {
        total_copies += cards[i]->copies;
        free_card(cards[i]);
    }

    free(cards);
    return total_copies;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        /*long int result = solve_part_1("input.txt");*/
        long int result = solve_part_2("input.txt");
        printf("Final result: %ld\n", result);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
