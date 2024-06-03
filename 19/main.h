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
    Part **data;
    size_t len;
} Parts;

void input_parse(const char *filename, Workflows **workflows_out, Parts **parts_out);
void workflows_free(Workflows *workflows);
void parts_free(Parts *parts);
