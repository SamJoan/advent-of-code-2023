
#include <stdint.h>

typedef struct {
    uint64_t time;
    uint64_t distance;
} Race;

typedef struct {
    Race ** races;
    int len;
} Paper;

Paper *paper_parse(char *filename);
uint64_t solve_part_1(char *filename);
void paper_free(Paper *p);
