#define _GNU_SOURCE
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int get_calibration_value(char *str) {
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

    } else {
        run_tests();
    }
}
