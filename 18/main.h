#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point **data;
    size_t len;
} Points;

uint64_t solve_part1(char *filename);
void points_free(Points *points);
Points *parse_points(const char *filename, char part);
