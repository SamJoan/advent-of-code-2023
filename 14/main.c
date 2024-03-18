#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Platform *platform_init() {
    Platform *p = smalloc(sizeof(Platform));
    p->len = 0;
    p->data = NULL;

    return p;
}

void platform_add(Platform *p, char *line) {
    p->len++;
    p->data = srealloc(p->data, sizeof(char *) * p->len);
    p->data[p->len - 1] = line;
}

void platform_free(Platform *p) {
    for(int i = 0; i < p->len; i++) {
        free(p->data[i]);
    }
    free(p->data);
    free(p);
}

Platform *parse_platform(const char *filename) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

        Platform *p = platform_init();

	fp = fopen(filename, "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
            platform_add(p, strndup(line, strlen(line) - 1));
	}

	fclose(fp);
	if (line)
	    free(line);

        return p;
}

void print_p(Platform *p) {
    for(int y = 0; y < p->len; y++) {
        printf("%s\n", p->data[y]);
    }

    printf("\n");
}

void tilt_north(Platform *p) {
    for(int x = 0; x < strlen(p->data[0]); x++) {
        int dest_y = -1;
        for(int y = 0; y < p->len; y++) {
            char c = p->data[y][x];
            if(c == '.' && dest_y == -1) {
                dest_y = y;
            } else if(c == '#') {
                dest_y = -1;
            } else if(c == 'O' && dest_y != -1) {
                p->data[dest_y][x] = 'O';
                p->data[y][x] = '.';
                dest_y++;
            }
        }
    }
}

void tilt_west(Platform *p) {
    for(int y = 0; y < p->len; y++) {
        int dest_x = -1;
        for(int x = 0; x < strlen(p->data[y]); x++) {
            char c = p->data[y][x];
            if(c == '.' && dest_x == -1) {
                dest_x = x;
            } else if(c == '#') {
                dest_x = -1;
            } else if (c == 'O' && dest_x != -1) {
                p->data[y][dest_x] = 'O';
                p->data[y][x] = '.';
                dest_x++;
            }
        }
    }
}

void tilt_south(Platform *p) {
    for(int x = 0; x < strlen(p->data[0]); x++) {
        int dest_y = -1;
        for(int y = p->len - 1; y >= 0; y--) {
            char c = p->data[y][x];
            if(c == '.' && dest_y == -1) {
                dest_y = y;
            } else if(c == '#') {
                dest_y = -1;
            } else if(c == 'O' && dest_y != -1) {
                p->data[dest_y][x] = 'O';
                p->data[y][x] = '.';
                dest_y--;
            }
        }
    }
}

void tilt_east(Platform *p) {
    for(int y = 0; y < p->len; y++) {
        int dest_x = -1;
        for(int x = strlen(p->data[y]) - 1; x >= 0; x--) {
            char c = p->data[y][x];
            if(c == '.' && dest_x == -1) {
                dest_x = x;
            } else if(c == '#') {
                dest_x = -1;
            } else if (c == 'O' && dest_x != -1) {
                p->data[y][dest_x] = 'O';
                p->data[y][x] = '.';
                dest_x--;
            }
        }
    }
}

uint64_t calculate_weight(Platform *p) {
    uint64_t multiplier = 1;
    uint64_t weight = 0;
    for(int y = p->len - 1; y >= 0; y--) {
        uint64_t nb_rocks = 0;
        for(int x = 0; x < strlen(p->data[y]); x++) {
            char c = p->data[y][x];
            if(c == 'O') {
                nb_rocks++;
            }
        }

        weight += nb_rocks * multiplier;

        multiplier++;
    }

    return weight;
}

uint64_t solve_part_1(char *filename) {
    Platform *p = parse_platform(filename);

    tilt_north(p);
    uint64_t weight = calculate_weight(p);
    
    platform_free(p);
    return weight;
}

uint64_t solve_part_2(char *filename) {
    Platform *p = parse_platform(filename);

    tilt_north(p);
    tilt_west(p);
    tilt_south(p);
    tilt_east(p);

    print_p(p);
    exit(1);

    uint64_t weight = calculate_weight(p);
    
    platform_free(p);
    return weight;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result_1 = solve_part_1("input.txt");

        printf("Part 1:\n%lu\n", result_1);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
