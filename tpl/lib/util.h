
#include <stdint.h>
#include <stdlib.h>

size_t parse_ints(char *numbers_in, uint64_t **numbers_ptr);
void *smalloc(size_t size);
void *srealloc(void *ptr, size_t size);
