#pragma once

#include <stdint.h>

// key can be any type just rewrite hashtable_hash
typedef struct
{
    uint64_t key;
    void* value;
} HashEntry_t;

typedef struct
{
    int size;
    int cap;
    HashEntry_t* body;
} HashTable_t;

uint64_t hashtable_hash(uint64_t key);

HashEntry_t* hashentry_create(int cap);

HashTable_t* hashtable_create();

void hashtable_put(HashTable_t* t, uint64_t key, void* value);

void* hashtable_get(HashTable_t* t, uint64_t key);

void hashtable_resize(HashTable_t* t, int cap);

void hashtable_destory(HashTable_t* t);

int hashtable_findindex(HashTable_t* t, uint64_t key);

int hashtable_delete(HashTable_t* t, uint64_t key);
