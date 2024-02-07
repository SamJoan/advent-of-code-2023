#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int64_t *val;
    size_t len;
} History;

typedef struct {
    History **histories;
    size_t len;
} Report;

Report *parse_report(const char *filename);
int64_t solve_part1(char *filename);
int64_t solve_part2(char *filename);
void report_free(Report *report);
