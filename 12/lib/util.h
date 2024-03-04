#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void **data;
    size_t len;
    uint64_t (*hash_func)(void *);
    bool (*eq_func)(void *, void *);
} HashMap;

HashMap *hashmap_init(size_t nb_elems, uint64_t (*hash_func)(void *), bool (*eq_func)(void *, void *));
bool hashmap_in(HashMap *h, void *elem);
uint64_t hash_char(const char* key);
void hashmap_add(HashMap *h, void *elem);
void hashmap_free(HashMap *h);

uint64_t hash_char(const char* key);

size_t parse_ints(char *numbers_in, uint64_t **numbers_ptr);
void *scalloc(size_t nmemb, size_t size);
void *smalloc(size_t size);
void *srealloc(void *ptr, size_t size);
