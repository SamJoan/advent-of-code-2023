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

void map_parse_name(Map *map, char *line) {
    map->name = strdup(strtok_r(line, " ", &line));
}

void tr_parse(TranslationRule *tr, char *line) {
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
    tr->dst_end = dst + range - 1;
    tr->src_start = src;
    tr->src_end = src + range - 1;
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
                    maps->len += 1;
                    maps->maps = realloc(maps->maps, sizeof(Map*) * maps->len);
                    maps->maps[maps->len - 1] = map;
                }

                map = malloc(sizeof(Map));
                map_parse_name(map, line);
                map->len = 0;
                map->rules = NULL;
            } else {
                bool empty_line = strlen(line) == 1;
                if(!empty_line) {
                    TranslationRule *tr = malloc(sizeof(TranslationRule));
                    tr_parse(tr, line);
                    map->len += 1;
                    map->rules = realloc(map->rules, sizeof(TranslationRule*) * map->len);
                    map->rules[map->len - 1] = tr;
                }
            }
        }
        i++;
    }

    maps->len += 1;
    maps->maps = realloc(maps->maps, sizeof(Map*) * maps->len);
    maps->maps[maps->len - 1] = map;

    fclose(fp);
    if (line)
        free(line);

    *seeds_out = seeds;
    *maps_out = maps;
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
