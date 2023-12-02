#include <stdbool.h>

struct GameSet {
    int red;
    int green;
    int blue;
};

struct Game {
    int id;
    int possible;

    long int power;

    int minimum_red;
    int minimum_green;
    int minimum_blue;
};

struct Game *parse_game(char *game_str);
void parse_game_id(struct Game *game, char *game_id);
void parse_game_set(struct GameSet *game_set, char *game_set_str_in);
bool game_set_possible(struct GameSet *game_set);
