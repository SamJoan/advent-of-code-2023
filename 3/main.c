#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void p(char *str) {
    printf("%s\n", str);
}

bool is_symbol(char c) {
    return !isdigit(c) && c != '.';
}

bool validate_part_number(char *prev_line, char *cur_line, char *next_line, int start, int end) {
    if(start > 0 && is_symbol(cur_line[start - 1])) {
        return true;
    }

    if(end < strlen(cur_line) && is_symbol(cur_line[end + 1])) {
        return true;
    }

    int start_scan_offset = start > 0 ? start - 1 : start;
    int end_scan_offset = end < strlen(cur_line) - 1 ? end + 1 : end;

    for(int i = start_scan_offset; i <= end_scan_offset; i++) {
        if(prev_line != NULL) {
            if(is_symbol(prev_line[i])) {
                return true;
            }
        }

        if(next_line != NULL) {
            if(is_symbol(next_line[i])) {
                return true;
            }
        }
    }

    return false;
}

long int find_part_numbers(char *prev_line, char *cur_line_in, char *next_line) {
    char *cur_line = strdup(cur_line_in);

    long int result = 0;
    int digit_start_index = -1;
    for(int i = 0; i < strlen(cur_line_in); i++) {
        char c = cur_line[i];
        bool parsing_digit = digit_start_index != -1;
        if(isdigit(c)) {
            if(!parsing_digit) {
                digit_start_index = i;
            }
        } else if(parsing_digit) {
            cur_line[i] = '\0';
            char *nb_str = cur_line + digit_start_index;
            int digit_end_index = digit_start_index + strlen(nb_str) - 1; 
            bool is_valid = validate_part_number(prev_line, cur_line_in, next_line, digit_start_index, digit_end_index);

            if(is_valid) {
                long int nb_val = 0;
                sscanf(nb_str, "%ld", &nb_val);
                result += nb_val;
            }

            digit_start_index = -1;
        }
    }

    free(cur_line);
    return result;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

        fp = fopen("input.txt", "r");
        /*fp = fopen("input_test.txt", "r")*/
	if (fp == NULL)
	    exit(EXIT_FAILURE);

        char *lines[140];

        int nb = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
            lines[nb] = strdup(line);

            nb++;
	}

        int nb_elems = nb - 1;

        long int total = 0;
        for(int i = 0; i <= nb_elems; i++) {
            int prev_elem = i == 0 ? -1 : i - 1;
            int next_elem = i + 1 > nb_elems ? - 1 : i + 1;

            char *prev_line = i == 0 ? NULL : lines[i - 1];
            char *cur_line = lines[i];
            char *next_line = i == nb_elems ? NULL : lines[i + 1];
            
            long int part_numbers = find_part_numbers(prev_line, cur_line, next_line);
            total += part_numbers;

            printf("%s%s %s -> %ld\n\n", prev_line, cur_line, next_line, part_numbers);
        }

	fclose(fp);
	if (line)
	    free(line);

        printf("Final result: %ld\n", total);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
