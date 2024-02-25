
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t parse_ints(char *numbers_in, uint64_t **numbers_ptr) {
    char *numbers_str = strdup(numbers_in);
    char *numbers_str_orig = numbers_str;
    
    uint64_t *numbers = NULL;
    char *token = NULL;
    size_t i = 0;
    while((token = strtok_r(numbers_str, " ", &numbers_str))) {
        numbers = realloc(numbers, (i + 1) * sizeof(uint64_t));

        sscanf(token, "%" SCNu64, &numbers[i]);

        i++;
    }

    free(numbers_str_orig);
    *numbers_ptr = numbers;

    return i;
}

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
