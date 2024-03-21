#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char operation;
    char *label;
    uint64_t value;
    char *orig;
} Step;

typedef struct {
    Step **data;
    size_t len;
} Steps;

typedef struct Lens {
    char *label;
    uint64_t value;
    struct Lens *next;
} Lens;

typedef struct {
    Lens **data;
} Boxes;

Steps *parse_steps(const char *filename);
uint64_t hash(char *input);
void steps_free(Steps *s);
uint64_t solve_part_1(char *file);
uint64_t solve_part_2(char *file);
