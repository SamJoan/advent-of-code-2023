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
    for(int i = 0; i < w->len; i++) {
        Rule *r = w->data[i];
        bool match = rule_evaluate(r, part);
        if(match == true) {
            if(strcmp(r->dest_name, "A") == 0) {
                return true;
            } else if(strcmp(r->dest_name, "R") == 0) {
                return false;
            } else {
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

Interval *interval_init(uint64_t start, uint64_t end) {
    Interval *interval = smalloc(sizeof(Interval));
    interval->start = start;
    interval->end = end;

    return interval;
}

Interval *interval_dup(Interval *orig) {
    Interval *new = interval_init(orig->start, orig->end);
    return new;
}

PartInterval *pi_init(PartInterval *orig) {
    PartInterval *pi = smalloc(sizeof(PartInterval));

    if(orig == NULL) {
        pi->x = interval_init(1, 4000);
        pi->m = interval_init(1, 4000);
        pi->a = interval_init(1, 4000);
        pi->s = interval_init(1, 4000);
    } else {
        pi->x = interval_dup(orig->x);
        pi->m = interval_dup(orig->m);
        pi->a = interval_dup(orig->a);
        pi->s = interval_dup(orig->s);
    }

    return pi;
}

void interval_free(Interval *i) {
    free(i);
}

void pi_free(PartInterval *pi) {
    interval_free(pi->x);
    interval_free(pi->m);
    interval_free(pi->a);
    interval_free(pi->s);

    free(pi);
}

Interval *pi_get_key(PartInterval *pi, char key) {
    Interval *value = NULL;
    if(key == 'x') {
        value = pi->x;
    } else if(key == 'm') {
        value = pi->m;
    } else if(key == 'a') {
        value = pi->a;
    } else if(key == 's') {
        value = pi->s;
    } else {
        printf("Unknown category %c\n", key);
        exit(1);
    }

    return value;
}

void pi_set_key(PartInterval *pi, char key, uint64_t start, uint64_t end) {
    Interval *i = interval_init(start, end);
    
    if(key == 'x') {
        free(pi->x);
        pi->x = i;
    } else if(key == 'm') {
        free(pi->m);
        pi->m = i;
    } else if(key == 'a') {
        free(pi->a);
        pi->a = i;
    } else if(key == 's') {
        free(pi->s);
        pi->s = i;
    }
}

void debug_interval(char key, Interval *interval) {
    if(interval == NULL) {
        printf("%c: NULL\n", key);
    } else {
        printf("%c: %lu %lu\n", key, interval->start, interval->end);
    }
}

void debug_pi(PartInterval *pi) {
    debug_interval('x', pi->x);
    debug_interval('m', pi->m);
    debug_interval('a', pi->a);
    debug_interval('s', pi->s);
}

Evaluation *rule_evaluate_interval(Rule *rule, PartInterval *pi) {
    Condition *cond = rule->condition;

    PartInterval *eval_true = pi_init(pi);
    PartInterval *eval_false = pi_init(pi);

    if(cond == NULL) {
        pi_free(eval_false);
        eval_false = NULL;
    } else {
        char key = cond->category;
        uint64_t val = cond->value;
        Interval *interval = pi_get_key(pi, key);
        
        if(cond->sign == '>') {
            pi_set_key(eval_true, key, val + 1, interval->end);
            pi_set_key(eval_false, key, interval->start, val);
        } else if(cond->sign == '<') {
            pi_set_key(eval_true, key, interval->start, val - 1);
            pi_set_key(eval_false, key, val, interval->end);
        } else {
            printf("Unknown sign %c\n", cond->sign);
            exit(1);
        }

    }

    Evaluation *eval = smalloc(sizeof(Evaluation));
    eval->eval_true = eval_true;
    eval->eval_false = eval_false;

    return eval;
}

uint64_t sum_total(PartInterval *pi) {
    uint64_t sum = 1;

    Interval *x = pi->x;
    Interval *m = pi->m;
    Interval *a = pi->a;
    Interval *s = pi->s;

    sum *= x->end + 1 - x->start;
    sum *= m->end + 1 - m->start;
    sum *= a->end + 1 - a->start;
    sum *= s->end + 1 - s->start;

    return sum;
}

void evaluation_free(Evaluation *eval) {
    pi_free(eval->eval_true);
    if(eval->eval_false != NULL) {
        pi_free(eval->eval_false);
    }

    free(eval);
}

uint64_t part_process_interval(HashMap *workflows, PartInterval *pi_in, Workflow *w) {
    PartInterval *next = pi_init(pi_in);

    uint64_t nb_accepted = 0;
    for(int i = 0; i < w->len; i++) {
        Rule *r = w->data[i];
        Evaluation *eval = rule_evaluate_interval(r, next);

        if(strcmp(r->dest_name, "A") == 0) {
            nb_accepted += sum_total(eval->eval_true);
        } else if(strcmp(r->dest_name, "R") != 0) {
            Workflow *next = hashmap_get(workflows, r->dest_name);
            nb_accepted += part_process_interval(workflows, eval->eval_true, next);
        }

        if(eval->eval_false != NULL) {
            pi_free(next);
            next = pi_init(eval->eval_false);
            evaluation_free(eval);
        } else {
            pi_free(next);
            evaluation_free(eval);
            break;
        }

    }

    return nb_accepted;
}

uint64_t solve_part2(char *filename) {
    HashMap *workflows = NULL;
    Parts *parts = NULL;
    input_parse(filename, &workflows, &parts);

    PartInterval *pi = pi_init(NULL);

    Workflow *in = hashmap_get(workflows, "in");
    uint64_t accepted_count = part_process_interval(workflows, pi, in);
    
    workflows_free(workflows);
    parts_free(parts);
    pi_free(pi);

    return accepted_count;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        char *filename = "input.txt";

        printf("Part 1:\n%lu\n", solve_part1(filename));
        printf("Part 2:\n%lu\n", solve_part2(filename));

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
