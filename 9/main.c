#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include "lib/util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

Report *report_init() {
    Report *report = calloc(1, sizeof(Report));

    return report;
}

void report_free(Report *report) {
    for(int i = 0; i < report->len; i++) {
        free(report->histories[i]->val);
        free(report->histories[i]);
    }

    free(report->histories);
    free(report);
}

History *history_init(int64_t *val, size_t len) {
    History *history = calloc(1, sizeof(History));
    history->val = val;
    history->len = len;

    return history;
}

Report *parse_report(const char *filename) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

        Report *report = report_init();

	while ((read = getline(&line, &len, fp)) != -1) {
            int64_t *ints = NULL;
            size_t nb_ints = parse_ints(line, &ints);

            History *history = history_init(ints, nb_ints);

            report->len++;
            report->histories = realloc(report->histories, report->len * sizeof(History*));
            report->histories[report->len - 1] = history;
            
	}

	fclose(fp);
	if (line)
	    free(line);

        return report;
}

void generate_rows(const History *history, int64_t ***rows_out, size_t *nb_rows_out) {
    int64_t **rows = NULL;
    bool all_vals_zero = false;

    int64_t *cur = history->val;
    size_t cur_len = history->len;

    size_t nb_rows = 0;
    while(!all_vals_zero) {
        nb_rows++;
        rows = realloc(rows, nb_rows * sizeof(int64_t**));
        rows[nb_rows - 1] = cur;

        int64_t *next_row = NULL;
        size_t next_row_len = 0;
        all_vals_zero = true;
        for(int i = 0; i < cur_len - 1; i++) {
            int64_t next_val = cur[i+1] - cur[i];
            if(next_val != 0) {
                all_vals_zero = false;
            }

            next_row_len++;
            next_row = realloc(next_row, next_row_len * sizeof(int64_t*));
            next_row[next_row_len - 1] = next_val;
        }

        cur = next_row;
        cur_len = next_row_len;
    }

    nb_rows++;
    rows = realloc(rows, nb_rows * sizeof(int64_t**));
    rows[nb_rows - 1] = cur;

    *rows_out = rows;
    *nb_rows_out = nb_rows;
}

int64_t history_extrapolate(History *history) {
    int64_t ** rows = NULL;
    size_t nb_rows = 0;
    generate_rows(history, &rows, &nb_rows);

    size_t first_row_len = history->len;

    int64_t prev = 0;
    for(int i = nb_rows - 2; i >= 0; i--) {
        size_t row_len = first_row_len - i;
        int64_t last_elem = rows[i][row_len - 1];
        prev = prev + last_elem;
    } for(int i = 1; i < nb_rows; i++) {
        free(rows[i]);
    }
    free(rows);

    return prev;
}

int64_t solve_part1(char *filename) {
    Report *report = parse_report(filename);
    int64_t sum = 0;
    for(int i = 0; i < report->len; i++) {
        History *history = report->histories[i];
        int64_t extrapolated = history_extrapolate(history);
        sum += extrapolated;
    }

    report_free(report);

    return sum;
}

int64_t history_extrapolate_previous(History *history) {
    int64_t ** rows = NULL;
    size_t nb_rows = 0;
    generate_rows(history, &rows, &nb_rows);

    size_t first_row_len = history->len;

    int64_t prev = 0;
    for(int i = nb_rows - 2; i >= 0; i--) {
        size_t row_len = first_row_len - i;
        int64_t first_elem = rows[i][0];
        prev = first_elem - prev;
    }

    for(int i = 1; i < nb_rows; i++) {
        free(rows[i]);
    }
    free(rows);

    return prev;
}

int64_t solve_part2(char *filename) {
    Report *report = parse_report(filename);
    int64_t sum = 0;
    for(int i = 0; i < report->len; i++) {
        History *history = report->histories[i];
        int64_t extrapolated = history_extrapolate_previous(history);
        sum += extrapolated;
    }

    report_free(report);

    return sum;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        int64_t result = solve_part1("input.txt");
        printf("Part 1: %lu\n", result);
        result = solve_part2("input.txt");
        printf("Part 2: %lu\n", result);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
