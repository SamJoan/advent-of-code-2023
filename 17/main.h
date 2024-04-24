#include <stdint.h>
#include <stdlib.h>

enum Direction {
    HORIZONTAL,
    VERTICAL
};

typedef struct {
    char **data;
    size_t len;
} Map;

typedef struct {
    int x;
    int y;
    enum Direction dir;
    int priority;
} Elem;

typedef struct {
    Elem **data;
    size_t len;
} PriorityQueue;

Map *map_parse(const char *filename);
void map_free(Map *map);
uint64_t solve_part1(char *filename);

Elem *elem_init(int priority, int x, int y, enum Direction dir);
Elem *pq_extract_min(PriorityQueue *pq);
PriorityQueue *pq_init();
void pq_free(PriorityQueue *pq);
void pq_insert(PriorityQueue *pq, Elem *elem);
