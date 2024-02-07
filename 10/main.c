#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
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

char up(const Map *map, int x, int y) {
    if(y - 1 < 0) {
        return 0;
    }

    return map->data[y - 1][x];
}

char down(const Map *map, int x, int y) {
    if(y + 1 >= map->len) {
        return 0;
    }

    return map->data[y + 1][x];
}

char left(const Map *map, int x, int y) {
    if(x - 1 < 0) {
        return 0;
    }

    return map->data[y][x - 1];
}

char right(const Map *map, int x, int y) {
    if(x + 1 > strlen(map->data[y]) - 1) {
        return 0;
    }

    return map->data[y][x + 1];
}

char determine_start_c(const Map *map, int x, int y) {
    printf("up: %c, right: %c, down %c, left %c\n", up(map, x, y), right(map, x, y), down(map, x, y), left(map, x, y));
    exit(1);
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

uint64_t solve_part_1(char *filename) {
    Map *map = map_parse(filename);
    int start_x = 0;
    int start_y = 0;
    char start_c = '\0';

    determine_start(map, &start_x, &start_y, &start_c);

    printf("%d %d\n", start_x, start_y);

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
