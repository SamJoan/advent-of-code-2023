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
    return !isdigit(c) && c != '.' && c != '\n' && c != '\r';
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

struct Number *get_number(char *line, int pos) {
    int start_pos = -1;
    int end_pos = -1;

    for(int i = pos; i >= 0; i--) {
        if(!isdigit(line[i])) {
            start_pos = i + 1;
            break;
        } else if(i == 0) {
            start_pos = 0;
        }
    }

    for(int i = pos; i < strlen(line); i++) {
        if(!isdigit(line[i])) {
            end_pos = i - 1;
            break;
        } else if(i == strlen(line) - 1) {
            end_pos = i;
        }
    }

    char *nb_str = malloc(end_pos - start_pos + 2);
    int index = 0;
    for(int i = start_pos; i <= end_pos; i++) {
        nb_str[index] = line[i];
        index++;
    }
    nb_str[index] = '\0';

    struct Number *nb = malloc(sizeof(struct Number));
    nb->start_pos = start_pos;
    nb->end_pos = end_pos;
    sscanf(nb_str, "%ld", &nb->number_value);
    free(nb_str);
    
    return nb;
}

long int calculate_gear_ratio(char *prev_line, char *cur_line, char *next_line, int index) {
    int start_index = index == 0 ? 0 : index - 1;
    int end_index = index + 1 == strlen(cur_line) ? index : index + 1;

    int max_number = 10;
    struct Number *number[max_number];
    for(int i = 0; i < max_number; i++) {
        number[i] = NULL;
    }

    int found = 0;
    for(int i = start_index; i <= end_index; i++) {
        char prev_c = prev_line[i];
        if(isdigit(prev_c)) {
            number[found] = get_number(prev_line, i);
            i = number[found]->end_pos;
            found++;
        }
    }

    for(int i = start_index; i <= end_index; i++) {
        char next_c = next_line[i];
        if(isdigit(next_c)) {
            number[found] = get_number(next_line, i);
            i = number[found]->end_pos;
            found++;
        }
    }

    if(index != 0 && isdigit(cur_line[index - 1])) {
        number[found] = get_number(cur_line, index - 1);
        found++;
    }

    if(index + 1 != strlen(cur_line) && isdigit(cur_line[index + 1])) {
        number[found] = get_number(cur_line, index + 1);
        found++;
    }

    long int result = 0;
    if(found == 2) {
         result = number[0]->number_value * number[1]->number_value;
    }

    for(int i = 0; i < max_number; i++) {
        free(number[i]);
    }

    return result;
}

long int find_gears(char *prev_line, char *cur_line_in, char *next_line) {
    long int gear_ratio = 0;
    for(int i = 0; i < strlen(cur_line_in); i++) {
        char c = cur_line_in[i];
        if(c == '*') {
            long int ratio = calculate_gear_ratio(prev_line, cur_line_in,
                    next_line, i);

            gear_ratio += ratio;
        }
    }

    return gear_ratio;
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
        /*fp = fopen("input_test.txt", "r");*/
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
        long int total_gear_ratio = 0;
        for(int i = 0; i <= nb_elems; i++) {
            int prev_elem = i == 0 ? -1 : i - 1;
            int next_elem = i + 1 > nb_elems ? - 1 : i + 1;

            char *prev_line = i == 0 ? NULL : lines[i - 1];
            char *cur_line = lines[i];
            char *next_line = i == nb_elems ? NULL : lines[i + 1];
            
            long int part_numbers = find_part_numbers(prev_line, cur_line, next_line);
            long int gear_ratio = find_gears(prev_line, cur_line, next_line);
            total += part_numbers;
            total_gear_ratio += gear_ratio;

            /*printf("%s%s %s -> %ld\n\n", prev_line, cur_line, next_line, part_numbers);*/
            /*printf("%ld\n", total);*/
        }

	fclose(fp);
	if (line)
	    free(line);

        printf("Final result: %ld, gear_ratio: %ld\n", total, total_gear_ratio);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
