#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_ints(char *filename, uint64_t **times, uint64_t **distances) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int nb = 0;
    char * token;
    int nb_races = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        char *line_dup = strdup(line);
        char *line_dup_orig = line_dup;
        int i = 0;
        while((token = strtok_r(line_dup, ":", &line_dup))) {
            if(i == 1) {
                if(nb == 0) {
                    nb_races = parse_ints(token, times);
                } else if(nb == 1) {
                    int ret = parse_ints(token, distances);
                    assert(ret == nb_races);
                }
            }
            i++;
        }

        free(line_dup_orig);
        nb++;
    }

    fclose(fp);
    if (line)
        free(line);

    return nb_races;
}

Paper *paper_init() {
    Paper *p = malloc(sizeof(Paper));
    p->len = 0;
    p->races = NULL;

    return p;
}

void paper_add(Paper *p, Race *r) {
    p->len++;
    p->races = realloc(p->races, sizeof(Race*) * p->len);
    p->races[p->len - 1] = r;
}

void paper_free(Paper *p) {
    for(int i = 0; i < p->len; i++) {
        free(p->races[i]);
    }

    free(p->races);
    free(p);
}

Paper *paper_parse(char *filename) {
    uint64_t *times;
    uint64_t *distances;

    int nb_races = read_ints(filename, &times, &distances);

    Paper *p = paper_init();
    for(int i = 0; i < nb_races; i++) {
        Race *r = malloc(sizeof(Race));
        r->time = times[i];
        r->distance = distances[i];

        paper_add(p, r);

    }

    free(times);
    free(distances);

    return p;
}

uint64_t calculate_distance(int t, uint64_t total_time) {
    uint64_t remaining_turns = total_time - t;
    uint64_t distance = t * remaining_turns;

    return distance;
}

uint64_t race_winnable_possibilities(Race * r) {
    uint64_t min_winnable = 0;
    for(int t = 0; t < r->time; t++) {
        if(calculate_distance(t, r->time) > r->distance) {
            min_winnable = t;
            break;
        }
    }

    uint64_t max_winnable = 0;
    for(int t = r->time; t >= 0; t--) {
        if(calculate_distance(t, r->time) > r->distance) {
            max_winnable = t;
            break;
        }
    }

    return max_winnable - min_winnable + 1;
}

uint64_t solve_part_1(char *filename) {
    Paper *p = paper_parse(filename);

    uint64_t result = 1;
    for(int i = 0; i < p->len; i++) {
        Race *r = p->races[i];
        uint64_t winnable = race_winnable_possibilities(r);
        result = result * winnable;
    }


    paper_free(p);
    return result;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        /*uint64_t result = solve_part_1("input.txt");*/
        uint64_t result = solve_part_1("input2.txt");
        printf("%lu\n", result);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
