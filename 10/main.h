#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char **data;
    size_t len;
} Map;

enum Dir {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};

typedef struct {
    enum Dir a;
    enum Dir b;
} Directions;


Map *map_parse(const char *filename);
uint64_t solve_part_1(char *filename);
void map_free(Map *map);
