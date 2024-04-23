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

            vd->data[UP] = false;
            vd->data[DOWN] = false;
            vd->data[LEFT] = false;
            vd->data[RIGHT] = false;
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

void move(int *x, int *y, enum Direction dir) {
    if(dir == RIGHT) {
        *x = *x + 1;
    } else if(dir == LEFT) {
        *x = *x - 1;
    } else if(dir == UP) {
        *y = *y - 1;
    } else if(dir == DOWN) {
        *y = *y + 1;
    } else {
        printf("Unknown direction\n");
        exit(1);
    }
}

bool out_of_bounds(int x, int y, Layout *layout) {
    size_t y_max = layout->len;
    size_t x_max = strlen(layout->data[0]);
    if(x < 0 || x >= x_max) {
        return true;
    }

    if(y < 0 || y >= y_max) {
        return true;
    }

    return false;
}

char *get_dir_str(enum Direction dir) {
    char *dir_str = "";
    if(dir == UP) {
        dir_str = "UP";
    } else if(dir == DOWN) {
        dir_str = "DOWN";
    } else if(dir == LEFT) {
        dir_str = "LEFT";
    } else if(dir == RIGHT) {
        dir_str = "RIGHT";
    } else {
        dir_str = "UNK";
    }

    return strdup(dir_str);
}

void navigate(int x, int y, enum Direction prev_dir, Layout *layout, Energised *energised, AlreadyVisited *av);

void move_navigate(int x, int y, enum Direction next_dir, Layout *layout, Energised *energised, AlreadyVisited *av) {
    int next_x = x;
    int next_y = y;
    move(&next_x, &next_y, next_dir);
    navigate(next_x, next_y, next_dir, layout, energised, av);
}

void av_add(AlreadyVisited *av, int x, int y, enum Direction dir) {
    av->data[y][x]->data[dir] = true;
}

bool av_visited_already(AlreadyVisited *av, int x, int y, enum Direction dir) {
    return av->data[y][x]->data[dir];
}

void navigate(int x, int y, enum Direction prev_dir, Layout *layout, Energised *energised, AlreadyVisited *av) {
    if(out_of_bounds(x, y, layout)) {
        return;
    }

    if(av_visited_already(av, x, y, prev_dir)) {
        return;
    } else {
        av_add(av, x, y, prev_dir);
    }

    energised->data[y][x] = '#';

    char c = layout->data[y][x];

    char *dir_str = get_dir_str(prev_dir);
    free(dir_str);

    int next_x = -1;
    int next_y = -1;
    if(c == '|') {
        if(prev_dir == LEFT || prev_dir == RIGHT) {
            move_navigate(x, y, UP, layout, energised, av);
            move_navigate(x, y, DOWN, layout, energised, av);
            return;
        }
    } else if(c == '-') {
        if(prev_dir == UP || prev_dir == DOWN) {
            move_navigate(x, y, LEFT, layout, energised, av);
            move_navigate(x, y, RIGHT, layout, energised, av);
            return;
        }
    } else if(c == '/') {
        if(prev_dir == RIGHT) {
            move_navigate(x, y, UP, layout, energised, av);
        } else if(prev_dir == UP) {
            move_navigate(x, y, RIGHT, layout, energised, av);
        } else if(prev_dir == DOWN) {
            move_navigate(x, y, LEFT, layout, energised, av);
        } else if(prev_dir == LEFT) {
            move_navigate(x, y, DOWN, layout, energised, av);
        }

        return;
    } else if(c == '\\') {
        if(prev_dir == UP) {
            move_navigate(x, y, LEFT, layout, energised, av);
        } else if(prev_dir == LEFT) {
            move_navigate(x, y, UP, layout, energised, av);
        } else if(prev_dir == DOWN) {
            move_navigate(x, y, RIGHT, layout, energised, av);
        } else if(prev_dir == RIGHT) {
            move_navigate(x, y, DOWN, layout, energised, av);
        }

        return;
    } else if (c != '.') {
        printf("Unknown character %c\n", c);
        exit(1);
    }
    
    next_x = x;
    next_y = y;
    move(&next_x, &next_y, prev_dir);
    navigate(next_x, next_y, prev_dir, layout, energised, av);
}

uint64_t solve(Layout *layout, int x, int y, enum Direction dir) {
    Energised *energised = energised_init(layout);
    AlreadyVisited *av = av_init(layout);

    navigate(x, y, dir, layout, energised, av); 

    uint64_t count = 0;
    for(int y = 0; y < energised->len; y++) {
        for(int x = 0; x < strlen(energised->data[y]); x++) {
            char c = energised->data[y][x];
            if(c == '#') {
                count++;
            }
        }
    }

    energised_free(energised);
    av_free(av);

    return count;
}

uint64_t solve_part1(char *filename) {
    Layout *layout = layout_parse(filename);
    uint64_t result = solve(layout, 0, 0, RIGHT);
    layout_free(layout);

    return result;
}

uint64_t solve_part2(char* filename) {
    Layout *layout = layout_parse(filename);

    uint64_t max = 0;
    uint64_t result = 0;
    for(int y = 0; y < layout->len; y++) {
        int x_left = 0;
        int x_right = strlen(layout->data[0]);

        result = solve(layout, x_left, y, RIGHT);
        if(result > max) {
            max = result;
        }

        result = solve(layout, x_right, y, LEFT);
        if(result > max) {
            max = result;
        }
    }

    for(int x = 0; x < strlen(layout->data[0]); x++) {
        int y_top = 0;
        int y_bottom = layout->len - 1;
        result = solve(layout, x, y_top, DOWN);
        if(result > max) {
            max = result;
        }

        result = solve(layout, x, y_bottom, UP);
        if(result > max) {
            max = result;
        }

    }

    layout_free(layout);

    return max;
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
