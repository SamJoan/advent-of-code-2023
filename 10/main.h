#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char **data;
    size_t len;
} Map;

enum Dir {DIR_UNSET, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};

typedef struct {
    enum Dir a;
    enum Dir b;
} Directions;

typedef struct {
    int x;
    int y;
} Coord;

typedef struct {
    Coord ** data;
    size_t len;
} Coords;


Map *map_parse(const char *filename);
uint64_t solve_part_1(char *filename);
uint64_t solve_part_2(char *filename);
void map_free(Map *map);
