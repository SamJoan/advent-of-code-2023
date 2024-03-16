#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char** data;
    size_t len;
} Pattern;

typedef struct {
    Pattern ** data;
    size_t len;
} Patterns;

typedef struct {
    int *data;
    size_t len;
} MirrorPos;

Patterns *parse_patterns(const char *filename);
void patterns_free(Patterns *p);
uint64_t solve_part_1(char *filename);
uint64_t solve_part_2(char *filename);
