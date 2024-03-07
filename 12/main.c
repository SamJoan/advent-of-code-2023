#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ConditionRecords *cr_init() {
    ConditionRecords *cr = smalloc(sizeof(ConditionRecords));
    cr->len = 0;
    cr->data = NULL;
    
    return cr;
}

void cr_add_record(ConditionRecords *cr, char *springs, int *damaged_spring_groups) {
    Record *record = smalloc(sizeof(Record));
    record->springs = springs;
    record->dsg = damaged_spring_groups;

    cr->len++;
    cr->data = srealloc(cr->data, sizeof(Record*) * cr->len);
    cr->data[cr->len - 1] = record;
}

void cr_free(ConditionRecords *cr) {
    for(int i = 0; i < cr->len; i++) {
        free(cr->data[i]->springs);
        free(cr->data[i]->dsg);
        free(cr->data[i]);
    }

    free(cr->data);
    free(cr);
}

void dsg_parse(char *dsg_string_in, int **dsg_out, bool unfold) {
    char *dsg_string = strdup(dsg_string_in);
    char *dsg_string_orig = dsg_string;
    char *token = NULL;

    int *ints = NULL;
    size_t len = 0;
    while((token = strtok_r(dsg_string, ",", &dsg_string))) {
        len++;
        ints = srealloc(ints, sizeof(int*) * len);
        sscanf(token, "%d", &ints[len - 1]);
    }

    if(!unfold) {
        ints = srealloc(ints, sizeof(int*) * len + 1);
        ints[len] = -1;
    } else {
        int arr_size = len * 5 + 1;
        ints = srealloc(ints, sizeof(int*) * arr_size);
        for(int i = 1; i < 5; i++) {
            int offset = i * len;
            for(int j = 0; j < len; j++) {
                int index = offset + j;
                int dsg = ints[index % len];

                ints[index] = dsg;
            }
        }

        ints[arr_size - 1] = -1;
    }

    *dsg_out = ints;
    free(dsg_string_orig);
}

size_t dsglen(int *dsg) {
    int *cur = dsg;
    size_t len = 0;
    while(*cur != -1) {
        len++;
        cur++;
    }

    return len;
}

uint64_t dsghash(int *dsg) {
    int *cur = dsg;
    size_t len = 0;
    uint64_t hash = 1;
    while(*cur != -1) {
        hash = hash * (*cur);

        len++;
        cur++;
    }

    return hash;
}

bool dsgcmp(int *dsg_a, int *dsg_b) {
    int *cur = dsg_a;
    size_t index = 0;
    while(*cur != -1) {
        if(dsg_a[index] != dsg_b[index]) {
            return false;
        }

        cur++;
        index++;
    }

    if(dsg_b[index] != -1) {
        return false;
    }

    return true;
}

ConditionRecords *parse_records(const char *filename, bool unfold) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

        ConditionRecords *records = cr_init();

	while ((read = getline(&line, &len, fp)) != -1) {
            char *line_dup = strdup(line);
            char *line_dup_orig = line_dup;
            char *token = NULL;
            int i = 0;
            char *springs = NULL;
            int *damaged_spring_groups = NULL;
            while((token = strtok_r(line_dup, " ", &line_dup))) {
                if(i == 0) {
                    springs = strdup(token);
                    if(unfold) {
                        size_t springs_len = strlen(springs);
                        size_t unfolded_len = (strlen(springs) * 5) + 5;
                        char *unfolded = malloc(unfolded_len);

                        int nb_question = 0;
                        for(int i = 0; i < 5; i++) {
                            int write_index =  i * springs_len + nb_question;
                            char *add = unfolded + write_index;
                            strncpy(add, springs, springs_len);
                            if(i != 4) {
                                unfolded[write_index + springs_len] = '?';
                                nb_question++;
                            } else {
                                unfolded[write_index + springs_len] = '\0';
                            }
                        }

                        free(springs);
                        springs = unfolded;

                    }
                } else if(i == 1) {
                    dsg_parse(token, &damaged_spring_groups, unfold);
                } else {
                    printf("Invalid line\n");
                    exit(1);
                }

                i++;
            }

            free(line_dup_orig);
            cr_add_record(records, springs, damaged_spring_groups);
	}

	fclose(fp);
	if (line)
	    free(line);

        return records;
}

bool cache_get(HashMap *cache, char *springs, int *dsg, int group_size, uint64_t *cached_result) {
    FunctionCall *f = function_call_init(springs, dsg, group_size);
    uint64_t *c = hashmap_get(cache, f);

    function_call_free(f);

    if(c != NULL) {
        *cached_result = *c;
        return true;
    } else {
        return false;
    }
}

void cache_put(HashMap *cache, char *springs, int *dsg, int group_size, uint64_t result) {
    FunctionCall *f = function_call_init(springs, dsg, group_size);

    uint64_t *result_ptr = malloc(sizeof(uint64_t));
    *result_ptr = result;

    hashmap_put(cache, f, result_ptr);
}

uint64_t valid_possibilities(HashMap *cache, char *springs, int *dsg, int group_size) {
    uint64_t cached_result = 0;
    bool cached = cache_get(cache, springs, dsg, group_size, &cached_result);
    if(cached) {
        return cached_result;
    }

    size_t springs_len = strlen(springs);
    size_t dsg_len = dsglen(dsg);

    if(springs_len == 0) {
        bool all_consumed = dsg_len == 0 && group_size == 0;
        bool can_consume_last = dsg_len == 1 && dsg[0] == group_size;
        uint64_t result = all_consumed || can_consume_last ? 1 : 0;

        cache_put(cache, springs, dsg, group_size, result);

        return result;
    }

    char symbols[3];
    if(springs[0] == '?') {
        symbols[0] = '#';
        symbols[1] = '.';
        symbols[2] = '\0';
    } else {
        symbols[0] = springs[0];
        symbols[1] = '\0';
    }

    uint64_t valid = 0;
    for(int i = 0; i < strlen(symbols); i++) {
        char s = symbols[i];
        if(s == '#') {
            valid += valid_possibilities(cache, springs+1, dsg, group_size+1); 
        }

        if(s == '.') {
            if(group_size != 0) {
                if(dsg_len > 0 && dsg[0] == group_size) {
                    valid += valid_possibilities(cache, springs+1, dsg + 1, 0);
                }
            } else {
                valid += valid_possibilities(cache, springs+1, dsg, 0);
            }
        }
    }

    cache_put(cache, springs, dsg, group_size, valid);
    return valid;
}

uint64_t solve_part_1(char *filename) {
    ConditionRecords *cr = parse_records(filename, false);
    uint64_t result = 0;
    for(int i = 0; i < cr->len; i++) {
        HashMap *cache = hashmap_init(50000, function_call_hash, function_call_cmp);
        Record *record = cr->data[i];
        result += valid_possibilities(cache, record->springs, record->dsg, 0);
        hashmap_free(cache);
    }

    cr_free(cr);
    return result;
}

uint64_t solve_part_2(char *filename) {
    ConditionRecords *cr = parse_records(filename, true);
    uint64_t result = 0;
    for(int i = 0; i < cr->len; i++) {
        HashMap *cache = hashmap_init(50000, function_call_hash, function_call_cmp);
        Record *record = cr->data[i];
        result += valid_possibilities(cache, record->springs, record->dsg, 0);
        hashmap_free(cache);
    }

    cr_free(cr);
    return result;
}

uint64_t function_call_hash(void *function_call_in) {
    FunctionCall *f = function_call_in;

    uint64_t hash = hash_char(f->springs);
    hash += dsghash(f->dsg);
    hash = hash * (f->group_size + 1);

    return hash;
}

bool function_call_cmp(void *func_a_in, void *func_b_in) {
    FunctionCall *func_a = func_a_in;
    FunctionCall *func_b = func_b_in;

    return strcmp(func_a->springs, func_b->springs) == 0 &&
        dsgcmp(func_a->dsg, func_b->dsg) &&
        func_a->group_size == func_b->group_size;
}

FunctionCall *function_call_init(char *springs, int *dsg, int group_size) {
    FunctionCall *f = smalloc(sizeof(FunctionCall));
    f->springs = springs;
    f->dsg = dsg;
    f->group_size = group_size;

    return f;
}

void function_call_free(FunctionCall *f) {
    free(f);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result1 = solve_part_1("input.txt");
        printf("Part 1:\n%lu\n", result1);

        uint64_t result2 = solve_part_2("input.txt");
        printf("Part 2:\n%lu\n", result2);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
