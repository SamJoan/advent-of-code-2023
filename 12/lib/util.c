
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

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

void *scalloc(size_t nmemb, size_t size) {
    void *p = calloc(nmemb, size);
    if(!p) {
        printf("Failed to allocate memory\n");
        exit(1);
    }

    return p;
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler?Noll?Vo_hash_function
uint64_t hash_char(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}

HashMap *hashmap_init(size_t max_size, uint64_t (*hash_func)(void *), bool (*eq_func)(void *, void *)) {
    HashMap *h = smalloc(sizeof(HashMap));

    h->data = scalloc(max_size, sizeof(KeyValue *));
    h->hash_func = hash_func;
    h->eq_func = eq_func;
    h->max_size = max_size;

    return h;
}

void hashmap_put(HashMap *h, void *key, void *value) {
    uint64_t cur = h->hash_func(key);
    KeyValue *kv = malloc(sizeof(KeyValue));

    kv->key = key;
    kv->value = value;

    while(true) {
        if(h->data[cur % h->max_size] == NULL) {
            h->data[cur % h->max_size] = kv;
            break;
        } else {
            cur++;
        }
    }
}

void *hashmap_get(HashMap *h, void *key) {
    uint64_t cur = h->hash_func(key);
    while(true) {
        KeyValue *kv = h->data[cur % h->max_size];
        if(kv != NULL) {
            if(h->eq_func(key, kv->key)) {
                return kv->value;
            } else {
                cur++;
            }
        } else {
            return NULL;
        }
    }
}

bool hashmap_in(HashMap *h, void *key) {
    void *ptr = hashmap_get(h, key);
    return ptr != NULL;
}

void hashmap_free(HashMap *h) {
    for(int i = 0; i < h->max_size; i++) {
        KeyValue *kv = h->data[i];
        if(kv != NULL) {
            free(kv->key);
            free(kv->value);
            free(kv);
        }
    }

    free(h->data);
    free(h);
}
