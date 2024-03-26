#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_CAPACITY 10
#define BALANCE_RATIO 0.7

uint64_t hashtable_hash(uint64_t key)
{
    return key;
}

HashEntry_t* hashentry_create(int cap)
{
    return (HashEntry_t*)calloc(cap, sizeof(HashEntry_t));
}

HashTable_t* hashtable_create()
{
    HashTable_t* t = (HashTable_t*)malloc(sizeof(HashTable_t));
    t->size = 0;
    t->cap = INITIAL_CAPACITY;
    t->body = hashentry_create(t->cap);
    return t;
}

int hashtable_findindex(HashTable_t* t, uint64_t key)
{
    int index = hashtable_hash(key) % t->cap;
    while (t->body[index].value != NULL && key != t->body[index].key)
    {
        index = (index + 1) % t->cap;
    }
    return index;
}

int hashtable_delete(HashTable_t* t, uint64_t key)
{
    int index = hashtable_findindex(t, key);
    if (t->body[index].value != NULL)
    {
        t->body[index].value = NULL;
        return 0;
    }
    return 1;
}

void hashtable_put(HashTable_t* t, uint64_t key, void* value)
{
    int index = hashtable_findindex(t, key);
    if (t->body[index].value != NULL)
    {
        t->body[index].value = value;
    }
    else
    {
        t->size += 1;
        if (t->cap * BALANCE_RATIO < t->size) {
            hashtable_resize(t, t->cap * 2);
            hashtable_put(t, key, value);
        }
        else {
            t->body[index].value = value;
            t->body[index].key = key;
        }
    }
}

void* hashtable_get(HashTable_t* t, uint64_t key)
{
    int index = hashtable_findindex(t, key);
    return t->body[index].value;
}

void hashtable_resize(HashTable_t* t, int cap)
{
    HashEntry_t* old = t->body;
    int old_cap = t->cap;
    t->body = hashentry_create(cap);
    t->cap = cap;
    for (int i = 0; i < old_cap; i++) {
        if (old[i].value != NULL) {
            hashtable_put(t, old[i].key, old[i].value);
        }
    }
    free(old);
}

void hashtable_destory(HashTable_t* t)
{
    free(t->body);
    free(t);
}
