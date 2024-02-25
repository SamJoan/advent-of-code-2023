#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Galaxy *galaxy_init() {
    Galaxy *galaxy = smalloc(sizeof(Galaxy));

    galaxy->data = NULL;
    galaxy->len = 0;

    return galaxy;
}

void galaxy_free(Galaxy *galaxy) {
    for(int i = 0; i < galaxy->len; i++) {
        free(galaxy->data[i]);
    }
    free(galaxy->data);
    free(galaxy);
}

void galaxy_add_line(Galaxy *galaxy, char *line) {
    galaxy->len++;
    galaxy->data = srealloc(galaxy->data, sizeof(char**) * galaxy->len);
    galaxy->data[galaxy->len - 1] = strndup(line, strlen(line));
}

Galaxy *galaxy_parse(const char *filename) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    Galaxy *galaxy = galaxy_init();

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        line[strlen(line) - 1] = '\0';
        galaxy_add_line(galaxy, line);
    }

    fclose(fp);
    if (line)
        free(line);

    return galaxy;
}

void galaxy_duplicate_col(Galaxy *galaxy, int dup_x) {
    int insert_index = 0;
    for(int y = 0; y < galaxy->len; y++) {
        char *line = galaxy->data[y];
        char *new_line = malloc(sizeof(char) * strlen(line) + 2);

        strncpy(new_line, line, dup_x);
        new_line[dup_x] = '.';
        strcpy(new_line + dup_x + 1, line + dup_x);

        free(line);
        galaxy->data[y] = new_line;

    }
}

Galaxy *galaxy_expand(Galaxy *galaxy) {
    Galaxy *expanded = galaxy_init();
    for(int y = 0; y < galaxy->len; y++) {
        char *line = galaxy->data[y];
        bool all_empty = true;
        for(int x = 0; x < strlen(line); x++) {
            if(galaxy->data[y][x] != '.') {
                all_empty = false;
                break;
            }
        }

        if(all_empty) {
            galaxy_add_line(expanded, line);
        }

        galaxy_add_line(expanded, line);

    }

    int duplicated = 0;
    for(int x = 0; x < strlen(galaxy->data[0]); x++) {
        bool all_empty = true;
        for(int y = 0; y < galaxy->len; y++) {
            if(galaxy->data[y][x] != '.') {
                all_empty = false;
            }
        }

        if(all_empty) {
            galaxy_duplicate_col(expanded, x + duplicated);
            duplicated++;
        }
    }

    return expanded;
}

Coords *coords_init() {
    Coords *coords = smalloc(sizeof(Coords));
    coords->len = 0;
    coords->data = NULL;

    return coords;
}

Coord *coord_init(int x, int y) {
    Coord *coord = smalloc(sizeof(Coord));
    coord->x = x;
    coord->y = y;
    
    return coord;
}

void coords_add_coord(Coords *coords, int x, int y) {
    Coord *coord = coord_init(x, y);

    coords->len++;
    coords->data = srealloc(coords->data, sizeof(Coord*) * coords->len);
    coords->data[coords->len - 1] = coord;
}

Coords *coords_get(Galaxy *galaxy) {
    Coords *coords = coords_init();
    for(int y = 0; y < galaxy->len; y++) {
        for(int x = 0; x < strlen(galaxy->data[y]); x++) {
            char c = galaxy->data[y][x];
            if(c == '#') {
                coords_add_coord(coords, x, y);
            }
        }
    }

    return coords;
}

void coords_free(Coords *coords) {
    for(int i = 0; i < coords->len; i++) {
        free(coords->data[i]);
    }

    free(coords->data);
    free(coords);
}

bool coord_eq(Coord *coord_a, Coord *coord_b) {
    return coord_a->x == coord_b->x && coord_a->y == coord_b->y;
}

CoordPairs *coord_pairs_init() {
    CoordPairs *coord_pair = smalloc(sizeof(CoordPairs));
    coord_pair->data = calloc(COORD_PAIRS_SIZE, sizeof(Pair*));
    
    if(coord_pair->data == NULL) {
        printf("Failed to allocate memory");
        exit(1);
    }

    return coord_pair;
}

uint hash_cp(Coord *coord_a, Coord *coord_b) {
    return ((coord_a->x + coord_a->y) * (coord_b->x + coord_b->y)) % COORD_PAIRS_SIZE;
}

void coord_pairs_add(CoordPairs *cp, Coord *coord_a, Coord *coord_b) {
    Pair * pair = smalloc(sizeof(Pair));
    pair->coord_a = coord_a;
    pair->coord_b = coord_b;
    pair->next = NULL;

    uint hash = hash_cp(coord_a, coord_b);
    if(cp->data[hash] == NULL) {
        cp->data[hash] = pair;
    } else {
        Pair *last = cp->data[hash];
        while(last != NULL) {
            if(last->next == NULL) {
                last->next = pair;
                last = NULL;
            } else {
                last = last->next;
            }
        }
    }
}


bool coord_pair_in(CoordPairs *cp, Coord *coord_a, Coord *coord_b) {
    uint hash = hash_cp(coord_a, coord_b);
    Pair *pair = cp->data[hash];
    while(pair != NULL) {
        if((coord_eq(coord_a, pair->coord_a) && coord_eq(coord_b, pair->coord_b)) ||
                (coord_eq(coord_a, pair->coord_b) && coord_eq(coord_b, pair->coord_a))) {
            return true;
        }

        pair = pair->next;
    }

    return false;
}

void coord_pairs_free(CoordPairs *pairs) {
    for(int i = 0; i < COORD_PAIRS_SIZE; i++) {
        Pair *pair = pairs->data[i];
        while(pair != NULL) {
            Pair *next = pair->next;
            free(pair);
            pair = next;
        }
    }

    free(pairs->data);
    free(pairs);
}

CoordPairs *coord_pairs_unique(Coords *coords) {
    CoordPairs *seen = coord_pairs_init();
    
    for(int i = 0; i < coords->len; i++) {
        Coord *coord_a = coords->data[i];
        for(int j = 0; j < coords->len; j++) {
            Coord *coord_b = coords->data[j];

            if(coord_eq(coord_a, coord_b)) {
                continue;
            }

            if(!coord_pair_in(seen, coord_a, coord_b)) {
                coord_pairs_add(seen, coord_a, coord_b);
            }
        }
    }

    return seen;
}

uint64_t distances_calculate(Coords *coords) {
    CoordPairs *pairs = coord_pairs_unique(coords);

    uint64_t total_distance = 0;
    for(int i = 0; i < COORD_PAIRS_SIZE; i++) {
        Pair *pair = pairs->data[i];
        while(pair != NULL) {
            Coord *coord_a = pair->coord_a;
            Coord *coord_b = pair->coord_b;

            uint64_t distance = labs(coord_a->x - coord_b->x) + labs(coord_a->y - coord_b->y);
            total_distance += distance;

            pair = pair->next;
        }
    }

    coord_pairs_free(pairs);

    return total_distance;
}

uint64_t solve_part_1(char *filename) {
    Galaxy *galaxy = galaxy_parse(filename);
    Galaxy *expanded = galaxy_expand(galaxy); 
    Coords *coords = coords_get(expanded);
    
    uint64_t total_distance = distances_calculate(coords);

    galaxy_free(galaxy);
    galaxy_free(expanded);
    coords_free(coords);

    return total_distance;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result = solve_part_1("input.txt");
        printf("Part 1:\n%lu\n", result);

        exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
