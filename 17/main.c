#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void map_free(Map *map) {
    for(int i = 0; i < map->len; i++) {
        free(map->data[i]);
    }

    free(map->data);
    free(map);
}

Map *map_init() {
    Map *map = smalloc(sizeof(Map));
    map->len = 0;
    map->data = NULL;

    return map;
}

Map *map_parse(const char *filename) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
        long int result = 0;

        Map *map = map_init();

	fp = fopen(filename, "r");
	if (fp == NULL)
	    exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
            map->len++;
            map->data = srealloc(map->data, map->len * sizeof(char*));
            map->data[map->len-1] = strndup(line, strlen(line) - 1);
	}

	fclose(fp);
	if (line)
	    free(line);

        return map;
}

PriorityQueue *pq_init() {
    PriorityQueue *pq = smalloc(sizeof(PriorityQueue));
    pq->data = NULL;
    pq->len = 0;
    pq->dirty = true;

    return pq;
}

Elem *elem_init(int priority, int x, int y, enum Direction dir) {
    Elem *elem = smalloc(sizeof(Elem));
    elem->x = x;
    elem->y = y;
    elem->dir = dir;
    elem->priority = priority;

    return elem;
}

void swap(Elem **a, Elem **b) {
    Elem *temp = *b;
    *b = *a;
    *a = temp;
}

void pq_heapify_min(PriorityQueue *pq, int i) {
    if(pq->len > 1) {
        int smallest = i;

        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if(l < pq->len && pq->data[l]->priority < pq->data[smallest]->priority) {
            smallest = l;
        }

        if(r < pq->len && pq->data[r]->priority < pq->data[smallest]->priority) {
            smallest = r;
        }

        if(smallest != i) {
            swap(&pq->data[i], &pq->data[smallest]);
            pq_heapify_min(pq, smallest);
        }
    } 
}

void pq_insert(PriorityQueue *pq, Elem *elem) {

    if(pq->len % 10000 == 0) {
        pq->data = srealloc(pq->data, (pq->len + 10000) * sizeof(Elem*));
    }

    pq->len++;
    pq->data[pq->len - 1] = elem;

    pq->dirty = true;
}

Elem *pq_extract_min(PriorityQueue *pq) {
    if(pq->len > 0) {
        if(pq->dirty) {
            for(int i = pq->len / 2 - 1; i >= 0; i--) {
                pq_heapify_min(pq, i);
            }

            pq->dirty = false;
        }

        Elem *elem = pq->data[0];

        swap(&pq->data[0], &pq->data[pq->len - 1]);
        pq->len--;

        pq_heapify_min(pq, 0);

        return elem;
    } else {
        return NULL;
    }
}

void pq_free(PriorityQueue *pq) {
    for(int i = 0; i < pq->len; i++) {
        free(pq->data[i]);
    }

    free(pq->data);
    free(pq);
}

VisitedDirections *vd_init() {
    VisitedDirections *vd = smalloc(sizeof(VisitedDirections));

    vd->data[VERTICAL] = false;
    vd->data[HORIZONTAL] = false;

    return vd;
}

AlreadyVisited *av_init(Map *l) {
    AlreadyVisited *av = smalloc(sizeof(AlreadyVisited));
    av->data = smalloc(sizeof(VisitedDirections**) * l->len);
    av->len = l->len;
    for(int y = 0; y < l->len; y++) {
        av->data[y] = smalloc(sizeof(VisitedDirections*) * (strlen(l->data[y]) + 1));
        size_t line_len = strlen(l->data[y]);
        for(int x = 0; x < line_len; x++) {
            VisitedDirections *vd = vd_init();
            av->data[y][x] = vd;
        }

        av->data[y][line_len] = NULL;
    }

    return av;
}

void av_free(AlreadyVisited *av) {
    for(int y = 0; y < av->len; y++) {
        int x = 0;
        VisitedDirections *next = NULL;
        while(((next = av->data[y][x]) != NULL)) {
            free(next);
            x++;
        }

        free(av->data[y]);
    }

    free(av->data);
    free(av);
}

uint64_t navigate(PriorityQueue *pq, AlreadyVisited *av, Map *map, int min_steps, int max_steps) {
    pq_insert(pq, elem_init(0, 0, 0, HORIZONTAL));
    pq_insert(pq, elem_init(0, 0, 0, VERTICAL));

    int max_x = strlen(map->data[0]) - 1;
    int max_y = map->len - 1;

    int max_cost = -1;
    while(true) {
        Elem *elem = pq_extract_min(pq);
        int x = elem->x;
        int y = elem->y;
        enum Direction dir = elem->dir;

        if(x == max_x && y == max_y) {
            max_cost = elem->priority;
            free(elem);
            return max_cost;
        }
        
        if(av->data[y][x]->data[dir]) {
            free(elem);
            continue;
        } else {
            av->data[y][x]->data[dir] = true;
        }

        int original_cost = elem->priority;
        int signs[] = {1, -1};
        for(int s = 0; s < 2; s++) {
            int sign = signs[s];

            int cost = original_cost;
            int new_x = x;
            int new_y = y;

            for(int i = 1; i <= max_steps; i++) {
                if(dir == HORIZONTAL) {
                    new_x = x + (i * sign);
                } else if(dir == VERTICAL) {
                    new_y = y + (i * sign);
                }

                if((new_x < 0 || new_x > max_x) ||
                        new_y < 0 || new_y > max_y) {
                    break;
                }

                cost += map->data[new_y][new_x] - '0';
                enum Direction new_dir = dir == HORIZONTAL ? VERTICAL : HORIZONTAL;

                if(!av->data[new_y][new_x]->data[new_dir] && i >= min_steps) {
                    pq_insert(pq, elem_init(cost, new_x, new_y, new_dir));
                }
            }

        }

        free(elem); 
    }
}

uint64_t solve_part1(char *filename) {
    PriorityQueue *pq = pq_init();
    Map *map = map_parse(filename);
    AlreadyVisited *av = av_init(map);

    uint64_t result = navigate(pq, av, map, 1, 3);
    map_free(map);
    
    pq_free(pq);
    av_free(av);

    return result;
}

uint64_t solve_part2(char *filename) {
    PriorityQueue *pq = pq_init();
    Map *map = map_parse(filename);
    AlreadyVisited *av = av_init(map);

    uint64_t result = navigate(pq, av, map, 4, 10);
    map_free(map);

    pq_free(pq);
    av_free(av);

    return result;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {
        uint64_t result_1 = solve_part1("input.txt");
        printf("Part 1:\n%lu\n", result_1);

        uint64_t result_2 = solve_part2("input.txt");
        printf("Part 2:\n%lu\n", result_2);

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
