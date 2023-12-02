#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void print(char *str) {
    printf("%s\n", str);
}

void parse_game_id(struct Game *game, char *game_id) {
    sscanf(game_id, "Game %d", &game->id);
    game->possible = -1;
}

void parse_game_set(struct GameSet *game_set, char *game_set_str_in) {
    char *game_set_str = strdup(game_set_str_in);
    char *saveptr;

    game_set->red = 0;
    game_set->green = 0;
    game_set->blue = 0;

    char *pos = strtok_r(game_set_str, ",", &saveptr);
    while(pos != NULL) {
        int red = 0;
        int green = 0;
        int blue = 0;

        if(strstr(pos, "red") != NULL) {
            sscanf(pos, " %d red", &red);
            game_set->red += red;
        } else if(strstr(pos, "green")) {
            sscanf(pos, " %d green", &green);
            game_set->green += green;
        } else if(strstr(pos, "blue")) {
            sscanf(pos, " %d blue", &blue);
            game_set->blue += blue;
        } else {
            assert(0);
        }

        pos = strtok_r(NULL, ",", &saveptr);
    }

    free(game_set_str);
}

bool game_set_possible(struct GameSet *game_set) {
    if(game_set->red > 12 || game_set->green > 13 || game_set->blue > 14) {
        return false;
    } else {
        return true;
    }
}

void calculate_minimum(struct Game *game, struct GameSet *game_set) {
    if(game_set->red > game->minimum_red) {
        game->minimum_red = game_set->red;
    }

    if(game_set->green > game->minimum_green) {
        game->minimum_green = game_set->green;
    }

    if(game_set->blue > game->minimum_blue) {
        game->minimum_blue = game_set->blue;
    }
}

void calculate_power(struct Game *game) {
    game->power = game->minimum_red * game->minimum_green * game->minimum_blue;
}

struct Game *parse_game(char *game_str_in) {
    char *game_str = strdup(game_str_in);
    char *saveptr;

    char *game_id_str = strtok_r(game_str, ":", &saveptr);

    struct Game *game = malloc(sizeof(struct Game));
    parse_game_id(game, game_id_str);

    struct GameSet *game_set = malloc(sizeof(struct GameSet));
    game->minimum_red = 0;
    game->minimum_green = 0;
    game->minimum_blue = 0;

    game->possible = true;
    char *next_set = strtok_r(NULL, ";", &saveptr);
    while(next_set != NULL) {
        parse_game_set(game_set, next_set);
        if(!game_set_possible(game_set)) {
            game->possible = false;
        }

        calculate_minimum(game, game_set);

        next_set = strtok_r(NULL, ";", &saveptr);
    }

    calculate_power(game);

    free(game_str);
    free(game_set);
    return game;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

	fp = fopen("input.txt", "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

        int total = 0;
        long int total_power = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
            struct Game *game = parse_game(line);
            if(game->possible == true) {
                total += game->id;
            }

            total_power += game->power;
            free(game);
	}

	fclose(fp);
	if (line)
	    free(line);

        printf("Final result: total: %d total_power: %ld\n", total,  total_power);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
