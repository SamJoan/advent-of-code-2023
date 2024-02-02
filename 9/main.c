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

History *history_init(uint64_t *val, size_t len) {
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
            uint64_t *ints = NULL;
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
