#include "lrucache.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

LRUCache_t* lrucache_create()
{
    LRUCache_t* cache = (LRUCache_t*)malloc(sizeof(LRUCache_t));
    cache->head = &cache->list[0];
    cache->tail = &cache->list[LRUCACHE_MAX_SIZE - 1];
    cache->table = hashtable_create();
    for (int i = 0; i < LRUCACHE_MAX_SIZE; i++)
    {
        cache->list[i].is_valid = 0;
        cache->list[i].prev = 0;
        cache->list[i].next = 0;
        if (i - 1 >= 0)
        {
            cache->list[i].prev = &cache->list[i - 1];
        }
        if (i + 1 < LRUCACHE_MAX_SIZE)
        {
            cache->list[i].next = &cache->list[i + 1];
        }
    }
    return cache;
}

int lrucache_destory(LRUCache_t* cache)
{
    hashtable_destory(cache->table);
    free(cache);
    return 0;
}

void lrucache_movehead(LRUCache_t* cache, LRUCacheNode_t* node)
{
    if (node != cache->head)
    {
        if (node->prev)
        {
            node->prev->next = node->next;
        }
        if (node->next)
        {
            node->next->prev = node->prev;
        }
        // node move to head
        if (cache->tail == node)
        {
            cache->tail = node->prev;
        }
        node->prev = NULL;
        cache->head->prev = node;
        node->next = cache->head;
        cache->head = node;
    }
}

int lrucache_put(LRUCache_t* cache, uint64_t key, const char* data, int len)
{
    if (len >= LRUCACHE_DATA_SIZE)
    {
        return 1;
    }
    // check if exist
    LRUCacheNode_t* node = hashtable_get(cache->table, key);
    if (node == NULL)
    {
        node = cache->tail;
        hashtable_put(cache->table, key, node);
        if (node->is_valid)
        {
            int ret = hashtable_delete(cache->table, node->key);
        }
    }
    // remove tail and move msg to head
    lrucache_movehead(cache, node);
    node->is_valid = 1;
    node->len = len;
    node->key = key;
    memmove(&node->data, data, len);

    return 0;
}

int lrucache_get(LRUCache_t* cache, uint64_t key, char** data, int* len)
{
    LRUCacheNode_t* node = hashtable_get(cache->table, key);
    if (NULL == node)
    {
        return 1;
    }
    lrucache_movehead(cache, node);
    *data = (char*)malloc(node->len);
    memmove(*data, &node->data, node->len);
    *len = node->len;
    return 0;
}
