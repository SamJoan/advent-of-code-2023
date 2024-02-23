#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char** data;
    size_t len;
} Galaxy;

Galaxy *parse_galaxy(const char *filename);

