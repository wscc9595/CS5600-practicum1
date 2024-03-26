#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "hashtable.h"
#include "randomcache.h"

RandomCache_t* rndcache_create()
{
    srand(time(0));
    RandomCache_t* cache = (RandomCache_t*)calloc(1, sizeof(RandomCache_t));
    cache->table = hashtable_create();
    cache->size = 0;
    return cache;
}

int rndcache_destory(RandomCache_t* cache)
{
    hashtable_destory(cache->table);
    free(cache);
    return 0;
}

int rndcache_put(RandomCache_t* cache, uint64_t key, const char* data, int len)
{
    if (len > RNDCACHE_DATA_SIZE)
        return 1;
    RandomCacheNode_t* node = hashtable_get(cache->table, key);
    if (node == NULL)
    {
        int index = 0;
        if (cache->size == RNDCACHE_MAX_SIZE)
        {
            index = rand() % RNDCACHE_MAX_SIZE;
        }
        else
        {
            index = cache->size;
            cache->size++;
        }
        node = &cache->list[index];
        if (node->is_valid)
        {
            hashtable_delete(cache->table, cache->list[index].key);
        }
        hashtable_put(cache->table, key, node);
    }

    node->is_valid = 1;
    node->len = len;
    node->key = key;
    memmove(node->data, data, len);
    return 0;
}

int rndcache_get(RandomCache_t* cache, uint64_t key, char** data, int* len)
{
    RandomCacheNode_t* node = hashtable_get(cache->table, key);
    if (node == NULL || node->is_valid == 0) {
        return 1;
    }
    *data = (char*)malloc(node->len);
    memmove(*data, &node->data, node->len);
    *len = node->len;
    return 0;
}
