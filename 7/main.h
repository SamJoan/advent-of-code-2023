#include <stdint.h>

typedef struct {
    char *cards;
    int len;
    uint64_t bid;
} Hand;

typedef struct {
    Hand **data;
    int len;
} Hands;

Hands *parse_hands(const char *filename);
uint64_t solve_part_1(const char *filename);
void hands_free(Hands *hands);
