#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Points *points_init() {
    Points *points = smalloc(sizeof(Points));

    points->len = 0;
    points->data = NULL;

    return points;
}

void points_add(Points *points, Point *point) {
    points->len++;
    points->data = srealloc(points->data, points->len * sizeof(Point*));
    points->data[points->len-1] = point;
}

Point *point_init(int x, int y) {
    Point *point = smalloc(sizeof(Point));
    point->x = x;
    point->y = y;

    return point;
}

void parse_line_p1(char *line, char *dir, int *steps) {
    sscanf(line, "%c %d", dir, steps);
}

void parse_line_p2(char *line_in, char *dir, int *steps) {
    char *line_dup = strdup(line_in);
    char *line_dup_orig = line_dup;
    char *token;

    int i = 0;
    while((token = strtok_r(line_dup, "(", &line_dup))) {
        if(i == 1) {
            char *hex_number = token + 1;
            char dir_char = hex_number[strlen(hex_number) - 3];
            hex_number[strlen(hex_number) - 3] = '\0';

            if(dir_char == '0') {
                *dir = 'R';
            } else if(dir_char == '1') {
                *dir = 'D';
            } else if(dir_char == '2') {
                *dir = 'L';
            } else if(dir_char == '3') {
                *dir = 'U';
            } else {
                printf("Unknown dir_char %c", dir_char);
                exit(1);
            }

            sscanf(hex_number, "%x", steps);
        }

        i++;
    }

    free(line_dup_orig);
}

void points_free(Points *points) {
    for(int i = 0; i < points->len; i++) {
        free(points->data[i]);
    }

    free(points->data);
    free(points);
}

Points *parse_points(const char *filename, char part) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

        Points *points = points_init();
        int x = 0;
        int y = 0;
        
        points_add(points, point_init(x, y));
	while ((read = getline(&line, &len, fp)) != -1) {
            char dir = '\0';
            int steps = -1;
            if(part == '1') {
                parse_line_p1(line, &dir, &steps);
            } else if(part == '2') {
                parse_line_p2(line, &dir, &steps);
            }

            if(dir == 'R') {
                x += steps;
            } else if(dir == 'L') {
                x -= steps;
            } else if(dir == 'U') {
                y -= steps;
            } else if(dir == 'D') {
                y += steps;
            }
            
            points_add(points, point_init(x, y));
	}

	fclose(fp);
	if (line)
	    free(line);

        return points;
}

uint64_t area_get(Points *points) {
    int64_t area = 0;
    for(int i = 0; i < points->len - 1; i++) {
        Point *cur = points->data[i];
        Point *next = points->data[i + 1];

        int64_t shoelace = (cur->x * next->y) + (cur->y * next->x * -1);
        area += shoelace;
    }

    return area / 2;
}

uint64_t perimeter_get(Points *points) {
    long perimeter = 0;
    for(int i = 0; i < points->len - 1; i++) {
        Point *cur = points->data[i];
        Point *next = points->data[i + 1];

        perimeter += labs(cur->x - next->x);
        perimeter += labs(cur->y - next->y);
    }

    return perimeter / 2;
}

uint64_t solve_part1(char *filename) {
    Points *points = parse_points(filename, '1');

    uint64_t area = area_get(points);
    uint64_t perimeter = perimeter_get(points);

    uint64_t result = area + perimeter + 1;

    points_free(points);

    return result;
}

uint64_t solve_part2(char *filename) {
    Points *points = parse_points(filename, '2');

    uint64_t area = area_get(points);
    uint64_t perimeter = perimeter_get(points);

    uint64_t result = area + perimeter + 1;

    points_free(points);

    return result;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result1 = solve_part1("input.txt");
        printf("Part 1: %lu\n", result1);

        uint64_t result2 = solve_part2("input.txt");
        printf("Part 2: %lu\n", result2);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
