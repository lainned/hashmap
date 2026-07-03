#define _GNU_SOURCE
#include "hashmap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static uint64_t hash_fnv1a_seeded(const char *str, uint64_t seed) {
    uint64_t hash = 14695981039346656037ULL ^ seed; // FNV offset basis, mixed with seed
    while (*str) {
        hash ^= (unsigned char)(*str++);
        hash *= 1099511628211ULL; // FNV prime
    }
    return hash;
}

static uint64_t random_uint64(){
    uint64_t seed;
    FILE *f = fopen("/dev/urandom", "rb");
    if(f){
        if(fread(&seed, sizeof(seed), 1, f) != 1){
            seed = 8512849123812931;
        }
        fclose(f);
    }
    else{
        seed = 8512849123812931;
    }
    return seed;
}

int hashmap_init(hashmap* hm){
    if(!hm) return -1;
    hm->size = 0;
    hm->capacity = HASHMAP_CAPACITY;
    hm->load_factor = HASHMAP_LOADFACTOR;
    hm->seed = random_uint64();
    hm->arr = calloc(hm->capacity, sizeof(entry*));

    if(!hm->arr) return -1;
    return 0;
}

hashmap* hashmap_new(){
    hashmap* hm = malloc(sizeof(hashmap));
    if(!hm) return NULL;
    hashmap_init(hm);
    if(!hm) return NULL;
    return hm;;
}

int hashmap_free(hashmap* hm){
    if(!hm || !hm->arr) return -1;
    for(int i = 0; i < hm->capacity; i++){
        entry* e = hm->arr[i];
        while(e){
            entry* next = e->next;
            free(e->key);
            free(e);
            e = next;
        }
    }
    free(hm->arr);
    hm->capacity = 0;
    hm->load_factor = 0;
    hm->size = 0;
    free(hm);
    return 0;
}
static int hashmap_resize(hashmap* hm){
    if(hm->size >= hm->load_factor * hm->capacity){
        size_t old_capacity = hm->capacity;
        hm->capacity *= 2;
        entry** new_arr = calloc(hm->capacity, sizeof(entry*));
        if(!new_arr) return -1;
        for(size_t i = 0; i < old_capacity; i++){
            entry* cur = hm->arr[i];
            while(cur != NULL){
                entry* next = cur->next;

                uint64_t hashed_key = hash_fnv1a_seeded(cur->key, hm->seed);
                int idx = hashed_key % hm->capacity;
                cur->next = new_arr[idx];
                new_arr[idx] = cur;

                cur = next;
            }
        }
        free(hm->arr);
        hm->arr = new_arr;
    }
    return 0;
}

int hashmap_set(hashmap* hm, const char* key, void* value){
    if(!hm || !hm->arr) return -1;
    uint64_t hashed_key = hash_fnv1a_seeded(key, hm->seed);

    size_t bucket = hashed_key % hm->capacity;
    entry* cur = hm->arr[bucket];
    while(cur != NULL){
        if(hashed_key == hash_fnv1a_seeded(cur->key, hm->seed) && strcmp(cur->key, key) == 0){
            cur->value = value;
            return 0;
        }
        cur = cur->next;
    }

    entry* new_entry = calloc(1, sizeof(entry));
    if(!new_entry) return -1;
    new_entry->key = strdup(key);
    if(!new_entry->key){
        return -1;
    }
    new_entry->value = value;
    
    new_entry->next = hm->arr[bucket];
    hm->arr[bucket] = new_entry;
    hm->size++;
    hashmap_resize(hm);
    
    return 0;
}

void* hashmap_get(hashmap* hm, const char* key){
    if(!hm || !hm->arr) return NULL;
    uint64_t hashed_key = hash_fnv1a_seeded(key, hm->seed);
    size_t idx = hashed_key % hm->capacity;
    entry* cur = hm->arr[idx];
    while(cur != NULL){
        if(hashed_key == hash_fnv1a_seeded(cur->key, hm->seed) && strcmp(cur->key, key) == 0){
            return cur->value;
        }
        cur = cur->next;
    }

    return NULL;
}
