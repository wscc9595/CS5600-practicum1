#pragma once
#include <stdint.h>
#include "hashtable.h"
#include "message.h"


// I implemented lrucache using hashtable and a fixed length double linked list.
// use message uid as hashtable's key and cachedatanode's ptr as hashtable's value
// so i can find the node in O(1), and move it to head in O(1).

// other design 
// 1. only double linked list
// find node will be O(N), which N is the size of cache, it's the easist way but get worst time complexity.
// 2. bst with double linked list
// find node will be O(log(N)), which N is the size of cache, hard to implement and time complexity is not better than hashtable

#define LRUCACHE_DATA_SIZE 1024

#define LRUCACHE_MAX_SIZE 16

typedef struct CacheData
{
    int is_valid;
    int len;
    uint64_t key;
    struct CacheData* prev;
    struct CacheData* next;
    char data[LRUCACHE_DATA_SIZE];
} LRUCacheNode_t;

typedef struct
{
    LRUCacheNode_t list[LRUCACHE_MAX_SIZE];
    HashTable_t *table;
    LRUCacheNode_t* head;
    LRUCacheNode_t* tail;
} LRUCache_t;

LRUCache_t* lrucache_create();

int lrucache_destory(LRUCache_t *cache);

int lrucache_put(LRUCache_t* cache, uint64_t key, const char* data, int len);

int lrucache_get(LRUCache_t* cache, uint64_t key, char** data, int* len);
