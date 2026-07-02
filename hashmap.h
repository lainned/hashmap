#pragma once

#include <stddef.h>
#include <stdint.h>
#define HASHMAP_CAPACITY 4096 // preferably choose a prime number or a power of two
#define HASHMAP_LOADFACTOR 0.7f

typedef struct entry{
    const char* key;
    void* value;
    struct entry *next;
} entry;

typedef struct hashmap{
    entry** arr;
    size_t capacity;
    size_t size;
    float load_factor;
    uint64_t seed;
} hashmap;


int hashmap_new(hashmap* hm);
int hashmap_free(hashmap* hm);
int hashmap_set(hashmap* hm, const char* key, void* value);