#pragma once

#include <stdint.h>
#include "message.h"

// I implemented lrucache using hashtable and a fixed length array.
// use message uid as hashtable's key and cachedatanode's ptr as hashtable's value
// so i can find the node in O(1), and replace in O(1).

// other design 
// 1. linked list or array
// find node will be O(N), which N is the size of cache, it's the easist way but get worst time complexity.
// 2. bst with linked list
// find node will be O(log(N)), which N is the size of cache, hard to implement and time complexity is not better than hashtable
#define RNDCACHE_DATA_SIZE 1024

#define RNDCACHE_MAX_SIZE 16

typedef struct 
{
    int is_valid;
    int len;
    uint64_t key;
    char data[RNDCACHE_DATA_SIZE];
} RandomCacheNode_t;


typedef struct
{
    RandomCacheNode_t list[RNDCACHE_MAX_SIZE];
    HashTable_t *table;
    int size;
} RandomCache_t;

RandomCache_t * rndcache_create();

int rndcache_destory(RandomCache_t *cache);

int rndcache_put(RandomCache_t* cache, uint64_t key, const char* data, int len);

int rndcache_get(RandomCache_t* cache, uint64_t key, char** data, int* len);

