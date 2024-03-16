#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Patterns *patterns_init() {
    Patterns *p = smalloc(sizeof(Patterns));
    p->data = NULL;
    p->len = 0;

    return p;
}

Pattern *pattern_init() {
    Pattern *p = smalloc(sizeof(Pattern));
    p->data = NULL;
    p->len = 0;
    
    return p;
}

void pattern_add_line(Pattern *p, char *line_in) {
    char *line = strndup(line_in, strlen(line_in) - 1);

    p->len++;
    p->data = srealloc(p->data, p->len * sizeof(char *));
    p->data[p->len - 1] = line;
}

void patterns_add_pattern(Patterns *patterns, Pattern *p) {
    patterns->len++;
    patterns->data = srealloc(patterns->data, patterns->len * sizeof(Pattern *));
    patterns->data[patterns->len - 1] = p;
}

void pattern_free(Pattern *p) {
    for(int i = 0; i < p->len; i++) {
        free(p->data[i]);
    }

    free(p->data);
    free(p);
}

void patterns_free(Patterns *p) {
    for(int i = 0; i < p->len; i++) {
        pattern_free(p->data[i]);
    }

    free(p->data);
    free(p);
}

Patterns *parse_patterns(const char *filename) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    Patterns *patterns = patterns_init();
    Pattern *p = pattern_init();
    while ((read = getline(&line, &len, fp)) != -1) {
        if(strlen(line) > 1) {
            pattern_add_line(p, line);
        } else {
            patterns_add_pattern(patterns, p);
            p = pattern_init();
        }
    }

    patterns_add_pattern(patterns, p);

    fclose(fp);
    if (line)
        free(line);

    return patterns;
}

MirrorPos *mirror_pos_init() {
    MirrorPos *mp = smalloc(sizeof(MirrorPos));
    mp->len = 0;
    mp->data = NULL;

    return mp;
}

void mirror_pos_add(MirrorPos *mp, int pos) {
    mp->len++;
    mp->data = srealloc(mp->data, sizeof(int) * mp->len);
    mp->data[mp->len - 1] = pos;
}

void mirror_pos_free(MirrorPos *mp) {
    free(mp->data);
    free(mp);
}

MirrorPos *find_mirror_str(char *str) {
    MirrorPos *mp = mirror_pos_init();
    int line_len = strlen(str);
    for(int x = 0; x < line_len - 1; x++) {

        int next = 1;
        bool possible_mirror = false;
        while(true) {
            int left = x - (next - 1);
            int right = x + next;
            if(left < 0 || right > line_len - 1) {
                if(possible_mirror) {
                    mirror_pos_add(mp, x);
                }

                break;
            }

            char a = str[left];
            char b = str[right];

            if(a == b) {
                possible_mirror = true;
                next++;
            } else {
                possible_mirror = false;
                break;
            }

        }
    }

    return mp;
}

void find_possible_mirrors(char *line, int *values) {
    MirrorPos *mp = find_mirror_str(line);

    for(int i = 0; i < mp->len; i++) {
        int pos = mp->data[i];
        values[pos]++;
    }

    mirror_pos_free(mp);
}

uint64_t find_mirrors_h(Pattern *p, uint64_t ignore_result) {
    int line_len = strlen(p->data[0]);
    int values[line_len];
    for(int i = 0; i < line_len; i++) {values[i] = 0;}

    for(int y = 0; y < p->len; y++) {
        find_possible_mirrors(p->data[y], values);
    }

    for(int i = 0; i < line_len; i++) {
        int val = values[i];
        if(val == p->len) {
            uint64_t potential_result = i + 1;
            if(potential_result != ignore_result) {
                return potential_result;
            } 
        }
    }

    return 0;
}

uint64_t find_mirrors_v(Pattern *p, uint64_t ignore_result) {
    int values[p->len];
    for(int i = 0; i < p->len; i++) {values[i] = 0;}
    int line_len = strlen(p->data[0]);

    for(int x = 0; x < line_len; x++) {
        char str[p->len + 1];
        for(int y = 0; y < p->len; y++) {
            str[y] = p->data[y][x];
        }
        str[p->len] = '\0';
        find_possible_mirrors(str, values);

    }

    uint64_t result = 0;
    for(int i = 0; i < p->len; i++) {
        int val = values[i];
        if(val == line_len) {
            uint64_t potential_result = (i + 1) * 100;
            if(potential_result != ignore_result) {
                return potential_result;
            }
        }
    }

    return 0;
}

uint64_t find_mirrors(Pattern *p, uint64_t ignore_result) {
    uint64_t mirror_value = 0;

    uint64_t mirror_value_h = find_mirrors_h(p, ignore_result);
    if(mirror_value_h != 0) {
        return mirror_value_h;
    } else {
        uint64_t mirror_value_v = find_mirrors_v(p, ignore_result);
        return mirror_value_v;
    }
}

uint64_t solve_part_1(char *filename) {
    Patterns *patterns = parse_patterns(filename);
    uint64_t result = 0;
    for(int i = 0; i < patterns->len; i++) {
        Pattern *p = patterns->data[i];
        result += find_mirrors(p, 0);
    }

    patterns_free(patterns);
    return result;
}

void flip_pattern(Pattern *p, int x, int y) {
    char orig = p->data[y][x];
    p->data[y][x] = orig == '.' ? '#' : '.';
}

uint64_t find_flipped_mirror_result(Pattern *p, uint64_t orig_result) {
    for(int y = 0; y < p->len; y++) {
        for(int x = 0; x < strlen(p->data[0]); x++) {
            flip_pattern(p, x, y);

            uint64_t potential_result = find_mirrors(p, orig_result);

            bool results_in_new_mirror = potential_result != orig_result && potential_result != 0;
            if(results_in_new_mirror)  {
                return potential_result;
            }

            flip_pattern(p, x, y);
        }
    }

    printf("Could not find any new mirror patterns by flipping patterns\n");
    exit(1);
}

uint64_t solve_part_2(char *filename) {
    Patterns *patterns = parse_patterns(filename);
    uint64_t result = 0;
    for(int i = 0; i < patterns->len; i++) {
        Pattern *p = patterns->data[i];
        uint64_t orig_result = find_mirrors(p, 0);

        result += find_flipped_mirror_result(p, orig_result);
    }

    patterns_free(patterns);
    return result;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result_1 = solve_part_1("input.txt");
        printf("Part 1:\n%lu\n", result_1);

        uint64_t result_2 = solve_part_2("input.txt");
        printf("Part 2:\n%lu\n", result_2);

        exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
