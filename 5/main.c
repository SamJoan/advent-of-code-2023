#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void seeds_parse(Seeds *seeds, char *line) {
    char *token;
    int i = 0;
    while((token = strtok_r(line, ":", &line))) {
        if(i == 1) {
            seeds->len = parse_ints(token, &seeds->seeds);
        }
        i++;
    }
}

void map_parse_name_line(Map *map, char *line) {
    map->name = strdup(strtok_r(line, " ", &line));
    map->len = 0;
    map->rules = NULL;
}

void tr_parse(Map *map, TranslationRule *tr, char *line) {
    char *token = NULL;
    uint64_t src = 0LLU;
    uint64_t dst = 0LLU;
    uint64_t range = 0LLU;
    int i = 0;
    while((token = strtok_r(line, " ", &line))) {
        if(i == 0) {
            sscanf(token, "%" SCNu64, &dst);
        } else if(i == 1) {
            sscanf(token, "%" SCNu64, &src);
        } else if(i == 2) {
            sscanf(token, "%" SCNu64, &range);
        } else {
            printf("Unknown index\n");
            exit(1);
        }

        i++;
    }

    tr->dst_start = dst;
    tr->src_start = src;
    tr->src_end = src + range - 1;

    map->len += 1;
    map->rules = realloc(map->rules, sizeof(TranslationRule*) * map->len);
    map->rules[map->len - 1] = tr;
}

void maps_add_map(Maps *maps, Map *map) {
    maps->len += 1;
    maps->maps = realloc(maps->maps, sizeof(Map*) * maps->len);
    maps->maps[maps->len - 1] = map;
}

void parse_almanac(const char* filename, Seeds **seeds_out, Maps **maps_out) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int i = 0;
    Seeds *seeds = malloc(sizeof(Seeds));
    Maps *maps = malloc(sizeof(Maps));
    maps->len = 0;
    maps->maps = NULL;
    
    Map *map = NULL;
    while ((read = getline(&line, &len, fp)) != -1) {
        if(i == 0) {
            seeds_parse(seeds, line);
        } else if (i > 1) {
            bool parsing_map_name = strchr(line, ':') != NULL;
            if(parsing_map_name) {
                if(map != NULL) {
                    maps_add_map(maps, map);
                }

                map = malloc(sizeof(Map));
                map_parse_name_line(map, line);
            } else {
                bool empty_line = strlen(line) == 1;
                if(!empty_line) {
                    TranslationRule *tr = malloc(sizeof(TranslationRule));
                    tr_parse(map, tr, line);
                }
            }
        }
        i++;
    }

    maps_add_map(maps, map);

    fclose(fp);
    if (line)
        free(line);

    *seeds_out = seeds;
    *maps_out = maps;
}

uint64_t tr_translate(const TranslationRule *tr, const uint64_t curr) {
    if(curr >= tr->src_start && curr <= tr->src_end) {
        return tr->dst_start + (curr - tr->src_start);
    }

    return curr;
}

uint64_t translate_to_location(const Maps *maps, const uint64_t seed) {
    uint64_t curr = seed;
    for(int i = 0; i < maps->len; i++) {
        Map *map = maps->maps[i];
        for(int j = 0; j < map->len; j++) {
            TranslationRule *tr = map->rules[j];
            uint64_t translation = tr_translate(tr, curr);
            if(translation != curr) {
                curr = translation;
                break;
            }
        }
    }

    return curr;
}

uint64_t solve_part_1(char *filename) {
    Seeds *seeds = NULL;
    Maps *maps = NULL;

    parse_almanac(filename, &seeds, &maps);

    uint64_t min_loc = 0;
    for(int i = 0; i < seeds->len; i++) {
        uint64_t seed = seeds->seeds[i];
        uint64_t location = translate_to_location(maps, seed);

        if(min_loc == 0 || location < min_loc) {
            min_loc = location;
        }
    }

    maps_free(maps);
    seeds_free(seeds);

    return min_loc;
}

Intervals *parse_intervals(Seeds *seeds) {
    Intervals *out = malloc(sizeof(Intervals));
    out->len = 0;
    out->vals = NULL;
    int nb = 0;
    for(int i = 0; i < seeds->len; i += 2) {
        out->len = nb + 1;
        out->vals = realloc(out->vals, out->len * (sizeof(Interval*)));

        uint64_t start = seeds->seeds[i];
        uint64_t range = seeds->seeds[i + 1];
        uint64_t end = start + range - 1;

        Interval *interval = malloc(sizeof(Interval));
        interval->start = start;
        interval->end = end;
        out->vals[nb] = interval;

        nb++;
    }

    return out;
}

void intervals_free(Intervals *intervals) {
    for(int i = 0; i < intervals->len; i++) {
        free(intervals->vals[i]);
    }

    free(intervals->vals);
    free(intervals);
}

uint64_t solve_part_2(char *filename) {
    Seeds *seeds = NULL;
    Maps *maps = NULL;

    parse_almanac(filename, &seeds, &maps);
    Intervals *intervals = parse_intervals(seeds);

    uint64_t min_loc = 0;
    uint64_t nb = 0;
    for(int i = 0; i < intervals->len; i++) {
        Interval *interval = intervals->vals[i];
        for(uint64_t seed = interval->start; seed <= interval->end; seed++) {
            if(nb % 100000 == 0 && nb != 0) {
                printf("%lu attempts\n", nb);
            }
            uint64_t location = translate_to_location(maps, seed);

            if(min_loc == 0 || location < min_loc) {
                min_loc = location;
            }
            nb++;
        }
    }

    maps_free(maps);
    seeds_free(seeds);
    intervals_free(intervals);

    return min_loc;
}

void maps_free(Maps *maps) {
    for(int i = 0; i < maps->len; i++) {
        for(int j = 0; j < maps->maps[i]->len; j++) {
            free(maps->maps[i]->rules[j]);
        }
        free(maps->maps[i]->rules);
        free(maps->maps[i]->name);
        free(maps->maps[i]);
    }
    free(maps->maps);
    free(maps);
}

void seeds_free(Seeds *seeds) {
    free(seeds->seeds);
    free(seeds);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        /*uint64_t result = solve_part_1("input.txt");*/
        uint64_t result = solve_part_2("input.txt");
        printf("%lu\n", result);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
