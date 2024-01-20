#include <stdint.h>

typedef struct {
    uint64_t src_start;
    uint64_t src_end;
    uint64_t dst_start;
    uint64_t dst_end;
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

void parse_almanac(const char* filename, Seeds **seeds, Maps **maps);
