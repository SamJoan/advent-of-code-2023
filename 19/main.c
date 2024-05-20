#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Workflows *workflows_init() {
    Workflows *workflows = smalloc(sizeof(Workflows));
    workflows->data = NULL;
    workflows->len = 0;

    return workflows;
}

Parts *parts_init() {
    Parts *parts = smalloc(sizeof(Parts));
    parts->data = NULL;
    parts->len = 0;

    return parts;
}

Workflow *workflow_init() {
    Workflow *workflow = smalloc(sizeof(Workflow));
    workflow->data = NULL;
    workflow->len = 0;
    workflow->name = NULL;

    return workflow;
}

void rules_parse(Workflow *workflow, char *rules_str_in) {
    char *rules_str = strndup(rules_str_in, strlen(rules_str_in) - 2);
    char *rules_str_orig = rules_str;

    while((token = strtok_r(rules_str, ",", &rules_str))) {
        printf("%s\n", token);
    }
    exit(1);
}

void workflow_parse(Workflows *workflows, char *line_in) {
    char *token = NULL;
    char *line = strdup(line_in);
    char *line_orig = line;

    Workflow *workflow = workflow_init();

    int nb = 0;
    char *name;
    while((token = strtok_r(line, "{", &line))) {
        if(nb == 0) {
            workflow->name = strdup(token);
        } else {
            rules_parse(workflow, token);
        }

        nb++;
    }

    free(line_orig);

    exit(1);
}

void part_parse(Parts *parts, char *line) {
    printf("part: %s", line);
}

void input_parse(const char *filename, Workflows **workflows_out, Parts **parts_out) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

        Workflows *workflows = workflows_init();
        Parts *parts = parts_init();

        bool parsing_parts = false;
	while ((read = getline(&line, &len, fp)) != -1) {

            if(parsing_parts) {
                part_parse(parts, line);
            } else {
                if(strcmp(line, "\n") == 0) {
                    parsing_parts = true;
                    continue;
                }

                workflow_parse(workflows, line);
            }
	}

	fclose(fp);
	if (line)
	    free(line);


        *parts_out = parts;
        *workflows_out = workflows;
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
