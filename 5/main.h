#include <stdint.h>

typedef struct {
    uint64_t src_start;
    uint64_t src_end;
    uint64_t dst_start;
} TranslationRule;

typedef struct {
    char *name;
    TranslationRule **rules;
    int len;
} Map;

typedef struct {
    uint64_t *seeds;
    int len;
} Seeds;

typedef struct {
    Map **maps;
    int len;
} Maps;

uint64_t solve_part_1(char *filename);
void maps_free(Maps *maps);
void parse_almanac(const char* filename, Seeds **seeds, Maps **maps);
void seeds_free(Seeds *seeds);
