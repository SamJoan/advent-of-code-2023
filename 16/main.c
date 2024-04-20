#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Layout *layout_init() {
    Layout *l = smalloc(sizeof(Layout));
    l->data = NULL;
    l->len = 0;

    return l;
}

void layout_add(Layout *l, char *line) {
    l->len++;
    l->data = srealloc(l->data, sizeof(char *) * l->len);
    l->data[l->len - 1] = line;
}

void layout_free(Layout *l) {
    for(int i = 0; i < l->len; i++) {
        free(l->data[i]);
    }

    free(l->data);
    free(l);
}

Layout *layout_parse(const char *filename) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    Layout *l = layout_init();

    while ((read = getline(&line, &len, fp)) != -1) {
        layout_add(l, strndup(line, strlen(line) - 1));
    }

    fclose(fp);
    if (line)
        free(line);

    return l;
}

Energised *energised_init(Layout *l) {
    Energised *energised = smalloc(sizeof(Energised));
    energised->data = smalloc(sizeof(char *) * l->len);
    energised->len = l->len;
    for(int i = 0; i < l->len; i++) {
        int line_len = strlen(l->data[i]) + 1;
        energised->data[i] = smalloc(line_len);
        for(int j = 0; j < line_len - 1; j++) {
            energised->data[i][j] = '.';
        }
        energised->data[i][line_len - 1] = '\0';
    }

    return energised;
}

void energised_free(Energised *energised) {
    for(int i = 0; i < energised->len; i++) {
        free(energised->data[i]);
    }

    free(energised->data);
    free(energised);
}

VisitedDirections *vd_init() {
    VisitedDirections *vd = smalloc(sizeof(VisitedDirections));
    return vd;
}

AlreadyVisited *av_init(Layout *l) {
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

uint64_t navigate(int x, int y, enum Direction dir, Layout *layout, Energised *energised, AlreadyVisited *av) {
    printf("%d %d %d\n", x, y, dir);
}

uint64_t solve_part1(char *filename) {
    Layout *layout = layout_parse(filename);
    Energised *energised = energised_init(layout);
    AlreadyVisited *av = av_init(layout);

    energised_free(energised);
    layout_free(layout);
    av_free(av);
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
