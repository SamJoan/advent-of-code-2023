#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "minunit.h"
#include "main.h"

MU_TEST(test_parse_game_id) {
    /*struct Game *game = parse_game("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green");*/
    struct Game *game = malloc(sizeof(struct Game));

    parse_game_id(game, "Game 1");

    mu_check(game->id == 1);
    mu_check(game->possible == -1);

    parse_game_id(game, "Game 13");
    mu_check(game->id == 13);

    parse_game_id(game, "Game 100");
    mu_check(game->id == 100);

    free(game);
}

MU_TEST(test_parse_set) {
    struct GameSet *game_set = malloc(sizeof(struct GameSet));

    parse_game_set(game_set, " 3 blue, 4 red");
    mu_check(game_set->blue == 3);
    mu_check(game_set->red == 4);
    mu_check(game_set_possible(game_set) == true);

    parse_game_set(game_set, " 1 red, 2 green, 6 blue");
    mu_check(game_set->blue == 6);
    mu_check(game_set->red == 1);
    mu_check(game_set->green == 2);
    mu_check(game_set_possible(game_set) == true);

    parse_game_set(game_set, " 2 green");
    mu_check(game_set->green == 2);
    mu_check(game_set_possible(game_set) == true);

    parse_game_set(game_set, " 1 red, 2 green, 6 blue, 14 red, 2 green");
    mu_check(game_set->blue == 6);
    mu_check(game_set->red == 15);
    mu_check(game_set->green == 4);
    mu_check(game_set_possible(game_set) == false);

    parse_game_set(game_set, " 1 red, 12 red");
    mu_check(game_set_possible(game_set) == false);

    parse_game_set(game_set, " 1 green, 13 green");
    mu_check(game_set_possible(game_set) == false);

    parse_game_set(game_set, " 1 blue, 14 blue");
    mu_check(game_set_possible(game_set) == false);

    parse_game_set(game_set, " 1 blue, 13 blue");
    mu_check(game_set_possible(game_set) == true);

    free(game_set);
}

MU_TEST(test_parse_game) {
    struct Game *game = parse_game("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green");
    mu_check(game->id == 1);
    mu_check(game->possible == true);
    mu_check(game->minimum_red == 4);
    mu_check(game->minimum_green == 2);
    mu_check(game->minimum_blue == 6);
    mu_check(game->power == 48);
    free(game);

    game = parse_game("Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue");
    mu_check(game->id == 2);
    mu_check(game->possible == true);
    mu_check(game->minimum_red == 1);
    mu_check(game->minimum_green == 3);
    mu_check(game->minimum_blue == 4);
    mu_check(game->power == 12);
    free(game);

    game = parse_game("Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red");
    mu_check(game->id == 3);
    mu_check(game->possible == false);
    mu_check(game->minimum_red == 20);
    mu_check(game->minimum_green == 13);
    mu_check(game->minimum_blue == 6);
    mu_check(game->power == 1560);
    free(game);

    game = parse_game("Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red");
    mu_check(game->id == 4);
    mu_check(game->possible == false);
    mu_check(game->minimum_red == 14);
    mu_check(game->minimum_green == 3);
    mu_check(game->minimum_blue == 15);
    mu_check(game->power == 630);
    free(game);

    game = parse_game("Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green");
    mu_check(game->id == 5);
    mu_check(game->possible == true);
    mu_check(game->minimum_red == 6);
    mu_check(game->minimum_green == 3);
    mu_check(game->minimum_blue == 2);
    mu_check(game->power == 36);
    free(game);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_parse_game_id);
    MU_RUN_TEST(test_parse_set);
    MU_RUN_TEST(test_parse_game);
}

int run_tests() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
