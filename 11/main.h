#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define COORD_PAIRS_SIZE 100000

typedef struct {
    char** data;
    size_t len;
} Galaxy;

typedef struct {
    int64_t x;
    int64_t y;
} Coord;

typedef struct {
    Coord **data;
    size_t len;
} Coords;

typedef struct Pair {
    Coord *coord_a;
    Coord *coord_b;
    struct Pair *next;
} Pair;

typedef struct {
    Pair **data;
} CoordPairs;

Coord *coord_init(int64_t x, int64_t y);
CoordPairs *coord_pairs_init();
Galaxy *galaxy_parse(const char *filename);
bool coord_eq(Coord *coord_a, Coord *coord_b);
bool coord_pair_in(CoordPairs *seen, Coord *coord_a, Coord *coord_b);
uint64_t hash_cp(Coord *coord_a, Coord *coord_b);
int64_t solve_part_1(char *filename);
void coord_pairs_add(CoordPairs *cp, Coord *coord_a, Coord *coord_b);
void coord_pairs_free(CoordPairs *pairs);
void galaxy_free(Galaxy *galaxy);
int64_t solve_part_2(char *filename, int64_t years);
