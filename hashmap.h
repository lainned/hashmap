#pragma once

#include <stddef.h>
#include <stdint.h>
#define HASHMAP_CAPACITY 4 // preferably choose  a power of two
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

int hashmap_init(hashmap* hm);
hashmap* hashmap_new();
int hashmap_free(hashmap* hm);
int hashmap_set(hashmap* hm, const char* key, void* value);
void* hashmap_get(hashmap* hm, const char* key);