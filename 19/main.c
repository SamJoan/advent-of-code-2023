#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <inttypes.h>
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

void rule_free(Rule *rule) {
    free(rule->condition);
    free(rule->dest_name);
    free(rule);
}

void workflow_free(Workflow *workflow) {
    for(int i = 0; i < workflow->len; i++) {
        rule_free(workflow->data[i]);
    }

    free(workflow->data);
    free(workflow->name);
    free(workflow);
}

void workflows_free(Workflows *workflows) {
    for(int i = 0; i < workflows->len; i++) {
        workflow_free(workflows->data[i]);
    }

    free(workflows->data);
    free(workflows);
}

void parts_free(Parts *parts) {
    for(int i = 0; i < parts->len; i++) {
        free(parts->data[i]);
    }

    free(parts->data);
    free(parts);
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

void workflow_add_rule(Workflow *workflow, Rule *rule) {
    workflow->len++;
    workflow->data = srealloc(workflow->data, sizeof(Rule*) * workflow->len);
    workflow->data[workflow->len - 1] = rule;
}

void rules_parse(Workflow *workflow, char *rules_str_in) {
    char *rules_str = strndup(rules_str_in, strlen(rules_str_in) - 2);
    char *rules_str_orig = rules_str;
    char *token = NULL;

    while((token = strtok_r(rules_str, ",", &rules_str))) {
        char *dststr = strchr(token, ':');
        bool contains_colon = dststr != NULL;
        
        Rule *r = smalloc(sizeof(Rule));
        if(contains_colon) {
            *dststr = '\0';

            Condition *cond = smalloc(sizeof(Condition));
            cond->category = token[0];
            cond->sign = token[1];
            sscanf(token + 2, "%" SCNu64, &cond->value);

            r->condition = cond;

        } else {
            r->condition = NULL;
        }

        r->dest_name = strdup(token);

        workflow_add_rule(workflow, r);
    }
    
    free(rules_str_orig);
}

void workflows_add_workflow(Workflows *workflows, Workflow *workflow) {
    workflows->len++;
    workflows->data = srealloc(workflows->data, sizeof(Workflow*) * workflows->len);
    workflows->data[workflows->len - 1] = workflow;
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

    printf("Finished parsing workflow %s, len %zu\n", workflow->name, workflow->len);
    workflows_add_workflow(workflows, workflow);
}

void parts_add_part(Parts *parts, Part *part) {
    parts->len++;
    parts->data = srealloc(parts->data, sizeof(Part*) * parts->len);
    parts->data[parts->len - 1] = part;
}

void part_parse(Parts *parts, char *line_in) {
    Part *p = smalloc(sizeof(Part));
    char *line = strdup(line_in);
    char *line_orig = line;

    line++;

    line[strlen(line) - 2] = '\0';

    char *token = NULL;
    while((token = strtok_r(line, ",", &line))) {
        char c = token[0];
        uint64_t val = 0;
        sscanf(token + 2, "%" SCNu64, &val);

        if(c == 'x') {
            p->x = val;
        } else if(c == 'm') {
            p->m = val;
        } else if(c == 'a') {
            p->a = val;
        } else if(c == 's') {
            p->s = val;
        } else {
            printf("Unknown c value %c\n", c);
            exit(1);
        }
    }

    printf("Finished parsing part: x: %lu, m: %lu, a: %lu, s: %lu\n", p->x, p->m, p->a, p->s);

    parts_add_part(parts, p);
    free(line_orig);
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
