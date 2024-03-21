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

#define BOXES_SIZE 256

uint64_t hash(char *input) {
    uint64_t cur = 0;
    for(int i = 0; i < strlen(input); i++) {
        int val = input[i] + cur;
        cur = (val * 17) % BOXES_SIZE;
    }

    return cur;
}

Steps *steps_init() {
    Steps *s = smalloc(sizeof(Steps));
    s->data = NULL;
    s->len = 0;

    return s;
}

Step *step_init(char *step_str) {
    char last_char = step_str[strlen(step_str) - 1];
    Step *step = smalloc(sizeof(Step));
    step->orig = strdup(step_str);

    if(last_char == '-') {
        step->operation = '-';
        step->label = strndup(step_str, strlen(step_str) - 1);
        step->value = 0;
    } else {
        step->operation = '=';
        char *step_str_dup = strdup(step_str);
        char *step_str_orig = step_str_dup;
        char *token = NULL;
        int i = 0;
        while((token = strtok_r(step_str_dup, "=", &step_str_dup))) {
            if(i == 0) {
                step->label = strdup(token);
            } else if(i == 1) {
                sscanf(token, "%" SCNu64, &step->value);
            } else {
                printf("Malformed string\n");
                exit(1);
            }

            i++;
        }

        free(step_str_orig);
    }

    return step;
}

void steps_add(Steps *s, char *step_str) {
    Step *step = step_init(step_str);

    s->len++;
    s->data = srealloc(s->data, sizeof(Step*) * s->len);
    s->data[s->len - 1] = step;
}

void steps_free(Steps *s) {
    for(int i = 0; i < s->len; i++) {
        free(s->data[i]->label);
        free(s->data[i]->orig);
        free(s->data[i]);
    }

    free(s->data);
    free(s);
}


Steps *parse_steps(const char *filename) {
        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        long int result = 0;

        Steps *steps = steps_init();

        fp = fopen(filename, "r");
        if (fp == NULL)
            exit(EXIT_FAILURE);

        while ((read = getline(&line, &len, fp)) != -1) {
            char *step;
            char *line_dup = strndup(line, strlen(line) - 1);
            char *line_dup_orig = line_dup;
            while((step = strtok_r(line_dup, ",", &line_dup))) {
                steps_add(steps, step);
            }

            free(line_dup_orig);
            break;
        }

        fclose(fp);
        if (line)
            free(line);

        return steps;
}

uint64_t solve_part_1(char *file) {
    Steps *s = parse_steps(file);
    uint64_t result = 0;
    for(int i = 0; i < s->len; i++) {
        Step *step = s->data[i];
        result += hash(step->orig);
    }

    steps_free(s);

    return result;
}

Boxes *boxes_init() {
    Boxes *b = smalloc(sizeof(Boxes));
    b->data = scalloc(sizeof(Lens*), BOXES_SIZE);

    return b;
}

void lens_free(Lens *l) {
    free(l);
}

Lens *lens_init(Step *step) {
    if(step->operation == '=') {
        Lens *lens = smalloc(sizeof(Lens));

        lens->label = step->label;
        lens->value = step->value;
        lens->next = NULL;

        return lens;
    } else {
        return NULL;
    }
}

void boxes_process_step(Boxes *boxes, Step *step) {
    uint64_t h = hash(step->label);
    Lens *l = lens_init(step);

    Lens *cur = boxes->data[h];
    Lens *prev = NULL;
    bool found = false;
    while(cur != NULL) {
        if(strcmp(cur->label, step->label) == 0) {
            found = true;
            Lens *replace = NULL;
            if(step->operation == '-') {
                replace = cur->next;
            } else {
                replace = l;
                l->next = cur->next;
            }

            if(prev == NULL) {
                boxes->data[h] = replace;
            } else {
                prev->next = replace;
            }

            lens_free(cur);
            break;
        }

        prev = cur;
        cur = cur->next;
    }

    if(!found && step->operation != '-') {
        if(prev != NULL) {
            prev->next = l;
        } else {
            boxes->data[h] = l;
        }
    }
}

void boxes_free(Boxes *b) {
    for(int i = 0; i < BOXES_SIZE; i++) {
        Lens *cur = b->data[i];
        while(cur != NULL) {
            Lens *next = cur->next;
            lens_free(cur);

            cur = next;
        }
    }
    
    free(b->data);
    free(b);

}

uint64_t solve_part_2(char *file) {
    Steps *s = parse_steps(file);
    Boxes *boxes = boxes_init();

    for(int i = 0; i < s->len; i++) {
        Step *step = s->data[i];
        boxes_process_step(boxes, step);
    }

    uint64_t focusing_power = 0;
    for(int i = 0; i < BOXES_SIZE; i++) {
        uint64_t box_nb = i + 1;
        Lens *cur = boxes->data[i];
        uint64_t slot_nb = 1;
        while(cur != NULL) {
            uint64_t pow = cur->value;
            focusing_power += box_nb * slot_nb * pow;

            cur = cur->next;
            slot_nb++;
        }
    }

    boxes_free(boxes);
    steps_free(s);

    return focusing_power;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result_1 = solve_part_1("input.txt");
        printf("Part 1:\n %lu\n", result_1);

        uint64_t result_2 = solve_part_2("input.txt");
        printf("Part 2:\n %lu\n", result_2);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
