#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Map *map_init() {
    Map *map = malloc(sizeof(Map));
    map->data = NULL;
    map->len = 0;

    return map;
}

void map_free(Map *map) {
    for(int i = 0; i < map->len; i++) {
        free(map->data[i]);
    }

    free(map->data);
    free(map);
}

Map *map_parse(const char *filename) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    Map *map = map_init();
    while ((read = getline(&line, &len, fp)) != -1) {
        map->len++;
        map->data = realloc(map->data, sizeof(char*) * map->len);
        map->data[map->len - 1] = strndup(line, strlen(line) - 1);
    }

    fclose(fp);
    if (line)
        free(line);

    return map;
}

void check_xy(const Map *map, int *x, int *y, bool *error) {
    if(*x < 0 || *y >= map->len || *y < 0 || *x > strlen(map->data[*y]) - 1) {
        *error = true;
    } else {
        *error = false;
    }
}

void u(const Map *map, const int cur_x, const int cur_y, int *x, int *y, bool *error) {
    *x = cur_x;
    *y = cur_y - 1;

    check_xy(map, x, y, error);
}

char up(const Map *map, int x, int y, bool *error) {
    int new_x = -1;
    int new_y = -1;
    u(map, x, y, &new_x, &new_y, error);

    if(!*error) {
        return map->data[new_y][new_x];
    } else {
        return '\0';
    }
}

void d(const Map *map, const int cur_x, const int cur_y, int *x, int *y, bool *error) {
    *x = cur_x;
    *y = cur_y + 1;

    check_xy(map, x, y, error);
}

char down(const Map *map, int x, int y, bool *error) {
    int new_x = -1;
    int new_y = -1;
    d(map, x, y, &new_x, &new_y, error);

    if(!*error) {
        return map->data[new_y][new_x];
    } else {
        return '\0';
    }
}

void l(const Map *map, const int cur_x, const int cur_y, int *x, int *y, bool *error) {
    *x = cur_x - 1;
    *y = cur_y;

    check_xy(map, x, y, error);
}

char left(const Map *map, int x, int y, bool *error) {
    int new_x = -1;
    int new_y = -1;
    l(map, x, y, &new_x, &new_y, error);

    if(!*error) {
        return map->data[new_y][new_x];
    } else {
        return '\0';
    }
}

void r(const Map *map, const int cur_x, const int cur_y, int *x, int *y, bool *error) {
    *x = cur_x + 1;
    *y = cur_y;

    check_xy(map, x, y, error);
}

char right(const Map *map, int x, int y, bool *error) {
    int new_x = -1;
    int new_y = -1;
    r(map, x, y, &new_x, &new_y, error);

    if(!*error) {
        return map->data[new_y][new_x];
    } else {
        return '\0';
    }
}

void char_dirs(Directions *dirs, char c) {
    Directions d = {DIR_UNSET, DIR_UNSET};
    if(c == '|') {
        d = (Directions) {DIR_UP, DIR_DOWN};
    } else if(c == '-') {
        d = (Directions) {DIR_LEFT, DIR_RIGHT};
    } else if(c == 'L') {
        d = (Directions) {DIR_UP, DIR_RIGHT};
    } else if(c == 'J') {
        d = (Directions) {DIR_UP, DIR_LEFT};
    } else if(c == '7') {
        d = (Directions) {DIR_LEFT, DIR_DOWN};
    } else if(c == 'F') {
        d = (Directions) {DIR_DOWN, DIR_RIGHT};
    }

    *dirs = d;
}

char *dir_user_friendly(enum Dir dir) {
    char *res = NULL;
    if(dir == DIR_UNSET) {
        res = strdup("UNSET");
    } else if(dir == DIR_UP) {
        res = strdup("UP");
    } else if(dir == DIR_DOWN) {
        res = strdup("DOWN");
    } else if(dir == DIR_LEFT) {
        res = strdup("LEFT");
    } else if(dir == DIR_RIGHT) {
        res = strdup("RIGHT");
    } else {
        printf("Invalid direction\n");
        exit(1);
    }

    return res;
}

void pdirs(Directions dirs) {
    char *a = dir_user_friendly(dirs.a);
    char *b = dir_user_friendly(dirs.b);

    printf("%s, %s\n", a, b);
    free(a);
    free(b);
}

bool dirs_contains(Directions dirs, enum Dir dir) {
    return dirs.a == dir || dirs.b == dir;
}

char guess_start_c(Directions start_c_dirs) {
    char *valid_chars = "|-LJ7F";
    for(int i = 0; i < strlen(valid_chars); i++) {
        Directions c_dirs = {DIR_UNSET, DIR_UNSET};
        char c = valid_chars[i];
        char_dirs(&c_dirs, c);

        if((start_c_dirs.a == c_dirs.a || start_c_dirs.a == c_dirs.b) &&
                (start_c_dirs.b == c_dirs.a || start_c_dirs.b == c_dirs.b)) {
            return c;
        }
    }

    printf("Can't guess starting location's character\n");
    exit(1);
}

char determine_start_c(const Map *map, int x, int y) {
    bool error = false;
    Directions dirs_up = {DIR_UNSET, DIR_UNSET};
    char_dirs(&dirs_up, up(map, x, y, &error));

    Directions dirs_down = {DIR_UNSET, DIR_UNSET};
    char_dirs(&dirs_down, down(map, x, y, &error));

    Directions dirs_left = {DIR_UNSET, DIR_UNSET};
    char_dirs(&dirs_left, left(map, x, y, &error));

    Directions dirs_right = {DIR_UNSET, DIR_UNSET};
    char_dirs(&dirs_right, right(map, x, y, &error));

    int valid_dirs = 0;
    enum Dir *start_dirs = calloc(4, sizeof(enum Dir));
    if(dirs_contains(dirs_up, DIR_DOWN)) {
        valid_dirs++;
        start_dirs[valid_dirs - 1] = DIR_UP;
    } 

    if(dirs_contains(dirs_down, DIR_UP)) {
        valid_dirs++;
        start_dirs[valid_dirs - 1] = DIR_DOWN;
    }

    if(dirs_contains(dirs_left, DIR_RIGHT)) {
        valid_dirs++;
        start_dirs[valid_dirs - 1] = DIR_LEFT;
    }

    if(dirs_contains(dirs_right, DIR_LEFT)) {
        valid_dirs++;
        start_dirs[valid_dirs - 1] = DIR_RIGHT;
    }

    assert(valid_dirs == 2);

    Directions c_dirs = {.a = start_dirs[0], .b = start_dirs[1]};

    free(start_dirs);

    return guess_start_c(c_dirs);
}

void determine_start(const Map *map, int *start_x, int *start_y, char *start_c) {
    for(int y = 0; y < map->len; y++) {
        for(int x = 0; x < strlen(map->data[0]); x++) {
            if(map->data[y][x] == 'S') {
                *start_y = y;
                *start_x = x;
                *start_c = determine_start_c(map, x, y);
                return;
            }
        }
    }

    printf("Couldn't find start pos\n");
    exit(1);
}

void move(const Map *map, const int x, const int y, const enum Dir dir, int* out_x, int *out_y) {
    bool error = false;
    if(dir == DIR_UP) {
        u(map, x, y, out_x, out_y, &error);
    } else if(dir == DIR_DOWN) {
        d(map, x, y, out_x, out_y, &error);
    } else if(dir == DIR_LEFT) {
        l(map, x, y, out_x, out_y, &error);
    } else if(dir == DIR_RIGHT) {
        r(map, x, y, out_x, out_y, &error);
    } else {
        printf("Invalid direction!\n");
        exit(1);
    }

    if(error) {
        printf("Attempted invalid move\n");
        exit(1);
    }
}

enum Dir opposite(enum Dir dir) {
    if(dir == DIR_DOWN) {
        return DIR_UP;
    } else if(dir == DIR_UP) {
        return DIR_DOWN;
    } else if(dir == DIR_LEFT) {
        return DIR_RIGHT;
    } else if(dir == DIR_RIGHT) {
        return DIR_LEFT;
    } else {
        printf("Invalid dir\n");
        exit(1);
    }
}

Coords *coords_init() {
    Coords *coords = malloc(sizeof(Coords));
    coords->data = NULL;
    coords->len = 0;
    
    return coords;
}

void coords_free(Coords *coords) {
    for(int i = 0; i < coords->len; i++) {
        free(coords->data[i]);
    }
    free(coords->data);
    free(coords);
}


void coords_add(Coords *coords, Coord *coord) {
    coords->len++;
    coords->data = realloc(coords->data, coords->len * sizeof(Coord*));
    coords->data[coords->len - 1] = coord;
}

Coord *coord_init(int x, int y) {
    Coord *coord = malloc(sizeof(Coord));
    coord->x = x;
    coord->y = y;
    
    return coord;
}

void map_navigate(int *steps_out, Coords **coords_out, Map *map, int start_x, int start_y, char start_c, Directions start_dirs) {
    int steps = 0;
    int cur_x = start_x;
    int cur_y = start_y;
    int cur_c = start_c;

    Coords *coords = coords_init();
    enum Dir cur_dir = start_dirs.a;
    while(steps == 0 || !(cur_x == start_x && cur_y == start_y)) {

        Coord *coord = coord_init(cur_x, cur_y);
        coords_add(coords, coord);

        Directions next_dirs = {DIR_UNSET, DIR_UNSET};
        char_dirs(&next_dirs, cur_c);

        enum Dir next_dir = opposite(cur_dir) == next_dirs.a ? next_dirs.b : next_dirs.a;
        int next_x = -1;
        int next_y = -1;
        move(map, cur_x, cur_y, next_dir, &next_x, &next_y);

        steps++;
        cur_x = next_x;
        cur_y = next_y;
        cur_c = map->data[cur_y][cur_x];
        cur_dir = next_dir;
    }

    Coord *coord = coord_init(cur_x, cur_y);
    coords_add(coords, coord);

    *steps_out = steps;
    *coords_out = coords;
}

uint64_t solve_part_1(char *filename) {
    Map *map = map_parse(filename);
    int start_x = 0;
    int start_y = 0;
    char start_c = '\0';
    determine_start(map, &start_x, &start_y, &start_c);

    Directions start_dirs = {DIR_UNSET, DIR_UNSET};
    char_dirs(&start_dirs, start_c);

    int steps = 0;
    Coords *coords = NULL;
    map_navigate(&steps, &coords, map, start_x, start_y, start_c, start_dirs);

    map_free(map);
    coords_free(coords);

    return steps / 2;
}

void map_simplify(Map *map, Map **simplified_map, Coords *coords) {
    Map *new_map = map_init();
    size_t x_len = strlen(map->data[0]) + 1;
    for(int y = 0; y < map->len; y++) {
        new_map->len++; 
        new_map->data = realloc(new_map->data, new_map->len * sizeof(char*));
        new_map->data[y] = malloc(x_len);
        new_map->data[y][x_len - 1] = '\0';
        for(int x = 0; x < x_len - 1; x++) {
            new_map->data[y][x] = ' ';
        }
    }

    for(int i = 0; i < coords->len; i++) {
        Coord *coord = coords->data[i];
        char c = map->data[coord->y][coord->x];
        if(c == 'S') {
            new_map->data[coord->y][coord->x] = determine_start_c(map, coord->x, coord->y);
        } else {
            new_map->data[coord->y][coord->x] = c;
        }
    }

    /*for(int i = 0; i < map->len; i++) {*/
        /*printf("'%s' %zu aa\n", map->data[i], strlen(map->data[i]));*/
    /*}*/

    *simplified_map = new_map;
}

void map_calculate_enclosed(const Map *map, uint64_t *enclosed_out) {
    uint64_t enclosed = 0;
    for(int y = 0; y < map->len; y++) {
        bool inside = false;
        for(int x = 0; x < strlen(map->data[y]); x++) {
            char c = map->data[y][x];
            if(c != ' ') {
                Directions dirs = {DIR_UNSET, DIR_UNSET};
                char_dirs(&dirs, c);
                if(dirs.a == DIR_UP || dirs.b == DIR_UP) {
                    inside = !inside;
                }
            } else if(inside) {
                enclosed++;
            }
        }
    }

    *enclosed_out = enclosed;
}

uint64_t solve_part_2(char *filename) {
    Map *map = map_parse(filename);
    int start_x = 0;
    int start_y = 0;
    char start_c = '\0';
    determine_start(map, &start_x, &start_y, &start_c);

    Directions start_dirs = {DIR_UNSET, DIR_UNSET};
    char_dirs(&start_dirs, start_c);

    int perimeter_steps = 0;
    Coords *coords = NULL;
    map_navigate(&perimeter_steps, &coords, map, start_x, start_y, start_c, start_dirs);
    Map *simplified_map = NULL;
    map_simplify(map, &simplified_map, coords);

    uint64_t enclosed_tiles = 0;
    map_calculate_enclosed(simplified_map, &enclosed_tiles);

    map_free(map);
    map_free(simplified_map);
    coords_free(coords);

    return enclosed_tiles;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result = solve_part_1("input.txt");
        printf("Part 1: %lu\n", result);
        result = solve_part_2("input.txt");
        printf("Part 2: %lu\n", result);

        exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
