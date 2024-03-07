#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void *key;
    void *value;
} KeyValue;

typedef struct {
    KeyValue **data;
    size_t max_size;
    uint64_t (*hash_func)(void *);
    bool (*eq_func)(void *, void *);
} HashMap;

HashMap *hashmap_init(size_t max_size, uint64_t (*hash_func)(void *), bool (*eq_func)(void *, void *));
uint64_t hash_char(const char* key);
bool hashmap_in(HashMap *h, void *key);
void* hashmap_get(HashMap *h, void *key);
void hashmap_put(HashMap *h, void *key, void *value);
void hashmap_free(HashMap *h);

uint64_t hash_char(const char* key);

size_t parse_ints(char *numbers_in, uint64_t **numbers_ptr);
void *scalloc(size_t nmemb, size_t size);
void *smalloc(size_t size);
void *srealloc(void *ptr, size_t size);
