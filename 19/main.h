#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char category;
    char sign;
    uint64_t value;
} Condition;

typedef struct {
    Condition *condition;
    char *dest_name;
} Rule;

typedef struct {
    Rule **data;
    size_t len;
    char *name;
} Workflow;

typedef struct {
    Workflow **data;
    size_t len;
} Workflows;

typedef struct {
    uint64_t x;
    uint64_t m; 
    uint64_t a;
    uint64_t s;
} Part;

typedef struct {
    uint64_t start;
    uint64_t end;
} Interval;

typedef struct {
    Interval *x;
    Interval *m; 
    Interval *a;
    Interval *s;
} PartInterval;

typedef struct {
    PartInterval *eval_true;
    PartInterval *eval_false;
} Evaluation;

typedef struct {
    Part **data;
    size_t len;
} Parts;

uint64_t solve_part1(char *filename);
void input_parse(const char *filename, HashMap **workflows_out, Parts **parts_out);
void parts_free(Parts *parts);
void workflows_free(HashMap *workflows);
uint64_t solve_part2(char *filename);
