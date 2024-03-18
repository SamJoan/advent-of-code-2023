#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char **data;
    size_t len;
} Platform;

Platform *parse_platform(const char *filename);
void platform_free(Platform *p);
uint64_t solve_part_1(char *filename);
uint64_t solve_part_2(char *filename);
