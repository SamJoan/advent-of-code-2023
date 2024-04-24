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
            map->data[map->len-1] = strdup(line);
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
    pq->len++;
    pq->data = srealloc(pq->data, pq->len * sizeof(Elem*));
    pq->data[pq->len - 1] = elem;

    for(int i = pq->len / 2 - 1; i >= 0; i--) {
        pq_heapify_min(pq, i);
    }
}

Elem *pq_extract_min(PriorityQueue *pq) {
    Elem *elem = pq->data[0];

    swap(&pq->data[0], &pq->data[pq->len - 1]);
    pq->len--;

    for(int i = pq->len / 2 - 1; i >= 0; i--) {
        pq_heapify_min(pq, i);
    }

    return elem;
}

void pq_free(PriorityQueue *pq) {
    for(int i = 0; i < pq->len; i++) {
        free(pq->data[i]);
    }

    free(pq->data);
    free(pq);
}

uint64_t solve_part1(char *filename) {
    PriorityQueue *pq = pq_init();
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s (test) <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "exec") == 0) {

	exit(EXIT_SUCCESS);
    } else {
        run_tests();
    }
}
