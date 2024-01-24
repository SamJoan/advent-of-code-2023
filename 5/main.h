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

typedef struct {
    uint64_t start;
    uint64_t end;
} Interval;

typedef struct {
    Interval **vals;
    int len;
} Intervals;

Interval *interval_init(uint64_t start, uint64_t end);
Intervals *intervals_init();
Intervals *map_apply_trs(Map *map, Intervals *intervals);
uint64_t solve_part_1(char *filename);
uint64_t solve_part_2(char *filename);
void intervals_add(Intervals *intervals, Interval *interval);
void intervals_free(Intervals *intervals);
void map_add_tr(Map *map, TranslationRule *tr);
void maps_free(Maps *maps);
void parse_almanac(const char* filename, Seeds **seeds, Maps **maps);
void seeds_free(Seeds *seeds);
void split_interval(Interval *interval, TranslationRule *tr, Interval **before, Interval **applied, Interval **after);
