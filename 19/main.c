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

uint64_t workflow_name_hash(void *name_in) {
    char *name = name_in;

    return hash_char(name);
}

bool workflow_cmp(void *key_a_in, void *key_b_in) {
    char *key_a = key_a_in;
    char *key_b = key_b_in;

    return strcmp(key_a, key_b) == 0;
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
}

void workflows_free(HashMap *workflows) {
    for(int i = 0; i < workflows->max_size; i++) {
        KeyValue *kv = workflows->data[i];
        if(kv != NULL) {
            workflow_free(kv->value);
        }
    }

    hashmap_free(workflows);
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
            r->dest_name = strdup(dststr+1);
        } else {
            r->condition = NULL;
            r->dest_name = strdup(token);
        }

        workflow_add_rule(workflow, r);
    }
    
    free(rules_str_orig);
}

void workflow_parse(HashMap *workflows, char *line_in) {
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

    hashmap_put(workflows, workflow->name, workflow);
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

    parts_add_part(parts, p);
    free(line_orig);
}

void input_parse(const char *filename, HashMap **workflows_out, Parts **parts_out) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

        HashMap *workflows = hashmap_init(50000, workflow_name_hash, workflow_cmp);

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

Workflow *worflow_find_initial(Workflows *w) {
    for(int i = 0; i < w->len; i++) {
        if(strcmp(w->data[i]->name, "in") == 0) {
            return w->data[i];
        }
    }

    printf("Could not find initial workflow\n");
    exit(1);
}

bool rule_evaluate(Rule *rule, Part *part) {
    Condition *cond = rule->condition;

    if(cond == NULL) {
        return true;
    } else {
        uint64_t left;
        if(cond->category == 'x') {
            left = part->x;
        } else if(cond->category == 'm') {
            left = part->m;
        } else if(cond->category == 'a') {
            left = part->a;
        } else if(cond->category == 's') {
            left = part->s;
        } else {
            printf("Unknown category %c\n", cond->category);
        }

        bool result;
        if(cond->sign == '>') {
            result = left > cond->value;
        } else if(cond->sign == '<') {
            result = left < cond->value;
        } else {
            printf("Unknown sign %c\n", cond->sign);
            exit(1);
        }

        /*printf("%ld %c %ld = %d\n", left, cond->sign, cond->value, result);*/
        return result;
    }
}

bool part_process(HashMap *workflows, Part *part, Workflow *w) {
    /*printf("Workflow %s\n", w->name);*/

    for(int i = 0; i < w->len; i++) {
        Rule *r = w->data[i];
        bool match = rule_evaluate(r, part);
        if(match == true) {
            if(strcmp(r->dest_name, "A") == 0) {
                return true;
            } else if(strcmp(r->dest_name, "R") == 0) {
                return false;
            } else {
                /*printf("Getting next workflow %s\n", r->dest_name);*/
                Workflow *next = hashmap_get(workflows, r->dest_name);
                return part_process(workflows, part, next);
            }
        }
    }

    printf("Unexpected end of workflow\n");
    exit(1);
}

uint64_t solve_part1(char *filename) {
    HashMap *workflows = NULL;
    Parts *parts = NULL;
    input_parse(filename, &workflows, &parts);


    uint64_t result = 0;
    Workflow *in = hashmap_get(workflows, "in");
    for(int i = 0; i < parts->len; i++) {
        Part *part = parts->data[i];
        bool accepted = part_process(workflows, part, in);
        if(accepted) {
            result += part->x + part->m + part->a + part->s;
        }

    }
    
    workflows_free(workflows);
    parts_free(parts);

    return result;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        char *filename = "input.txt";

        printf("Part 1:\n%lu\n", solve_part1(filename));

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
