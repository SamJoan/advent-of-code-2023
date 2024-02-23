#define _GNU_SOURCE
#include "lib/util.h"
#include "main.h"
#include "main_test.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *smalloc(size_t size) {
    void *p = malloc(size);
    if(!p) {
        printf("Failed to allocate memory\n");
        exit(1);
    }

    return p;
}

void *srealloc(void *ptr, size_t size) {
    void *p = realloc(ptr, size);
    if(!p) {
        printf("Failed to allocate memory\n");
        exit(1);
    }

    return p;
}

Galaxy *galaxy_init() {
    Galaxy *galaxy = smalloc(sizeof(Galaxy));

    galaxy->data = NULL;
    galaxy->len = 0;

    return galaxy;
}

Galaxy *parse_galaxy(const char *filename) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int result = 0;

    Galaxy *galaxy = galaxy_init();

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        galaxy->len++;
        galaxy->data = srealloc(galaxy->data, sizeof(char**) * galaxy->len);
        galaxy->data[galaxy->len - 1] = strdup(line);
    }

    fclose(fp);
    if (line)
        free(line);


    return galaxy;
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
