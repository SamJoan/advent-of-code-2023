#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct {
    char **data;
    size_t len;
} Layout;

typedef struct {
    char **data;
    size_t len;
} Energised;

typedef struct {
    bool data[4];
} VisitedDirections;

typedef struct {
    VisitedDirections ***data;
    size_t len;
} AlreadyVisited;

Layout *layout_parse(const char *filename);
void layout_free(Layout *l);
uint64_t solve_part1(char *filename);
uint64_t solve_part2(char* filename);
