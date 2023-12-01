#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *replace_str(char *input_str) {
    char originals[][10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    char replacements[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    char *pos = NULL;
    int relative_pos = -1;

    int min_pos = -1;
    int max_pos = -1;

    int min_pos_rep_index = -1;
    int max_pos_rep_index = -1;
    char *tmp = strdup(input_str);
    for(int i = 0; i < 10; i++) {
        char *orig = originals[i];
        pos = strstr(input_str, orig);

        if(pos != NULL) {
            relative_pos = pos - input_str;
            
            tmp[relative_pos] = replacements[i];

            if(min_pos == -1 || relative_pos < min_pos) {
                min_pos = relative_pos;
                min_pos_rep_index = i;
            }

            while(pos != NULL) {
                relative_pos = pos - input_str;
                tmp[relative_pos] = replacements[i];

                char *next_pos = strstr(pos + 1, orig);
                int is_last = next_pos == NULL;

                if(is_last && relative_pos > max_pos) {
                    max_pos = relative_pos;
                    max_pos_rep_index = i;
                }

                pos = next_pos;
            }
        }
    }

    if(max_pos_rep_index == -1 || min_pos_rep_index == -1) {
        return tmp;
    }

    return tmp;
}

long int get_calibration_value(char *input_str) {
    char *str = replace_str(input_str);

    int first_number = -1;
    int last_number = -1;

    for(int i = 0; i < strlen(str); i++) {
        char c = str[i];
        if(first_number == -1 && isdigit(c)) {
            first_number = c;
        } else if(isdigit(c)) {
            last_number = c;
        }
    }

    if(last_number == -1) {
        last_number = first_number;
    }

    char result_str[3];
    result_str[0] = first_number;
    result_str[1] = last_number;
    result_str[2] = '\0';

    long int result_int = -1;
    result_int = strtol(result_str, NULL, 10);

    free(str);

    return result_int;

}

long int sum_calibration_values(char *lines_in) {
    char *lines = strdup(lines_in);

    char *line = strtok(lines, "\n");
    long int result = 0;
    while(line != NULL) {
        long int calibration_value = get_calibration_value(line);
        result += calibration_value;
        line = strtok(NULL, "\n");
    }

    free(lines);

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
	if (fp == NULL)
	    exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
            long int calibration_value = get_calibration_value(line);
            result += calibration_value;
	}

	fclose(fp);
	if (line)
	    free(line);

        printf("Final result: %ld\n", result);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
