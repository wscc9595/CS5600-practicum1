#include "store.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hashtable.h"
#define ERROR(info, ...)                                                   \
    {                                                                      \
        fprintf(stderr, "ERROR: %s,%d,%s ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, info, ##__VA_ARGS__);                              \
        fprintf(stderr, "\n");                                             \
    }

const int MAGIC_NUM = 0x1234;
const char * suf_index = ".index";

Store_t* store_open(const char* s_path)
{
    Store_t* store = (Store_t*)malloc(sizeof(Store_t));
    store->hash_index = hashtable_create();
    char *db_path = malloc(strlen(s_path) + strlen(suf_index) + 1);
    memmove(db_path, s_path, strlen(s_path) + 1);
    strcat(db_path, suf_index);
    int fd = open(db_path, O_RDWR, 0644);
    int magic;
    if (fd >= 0)
    {
        // exist, check if magic equal MAGIC_NUM
        if (read(fd, &magic, 4) != 4 || magic != MAGIC_NUM)
        {
            free(db_path);
            free(store->hash_index);
            free(store);
            ERROR("read index magic number wrong expect(%d) get(%d)", MAGIC_NUM,
                  magic);
            close(fd);
            return NULL;
        }
        // load data
        StoreIndex_t index;
        while (read(fd, &index, sizeof(index)) > 0)
        {
            StoreIndex_t* tindex = (StoreIndex_t*)malloc(sizeof(StoreIndex_t));
            memmove(tindex, &index, sizeof(StoreIndex_t));
            hashtable_put(store->hash_index, index.key, tindex);
        }
    }
    else
    {
        // create new index
        fd = open(db_path, O_CREAT | O_RDWR, 0644);
        // write magic
        write(fd, &MAGIC_NUM, sizeof(MAGIC_NUM));
    }
    free(db_path);
    store->index_fd = fd;
    store->fd = open(s_path, O_CREAT | O_RDWR, 0644);
    return store;
}

int store_close(Store_t* store)
{
    for (int i = 0; i < store->hash_index->cap; i++)
    {
        free(store->hash_index->body[i].value);
    }
    hashtable_destory(store->hash_index);
    close(store->fd);
    close(store->index_fd);
    free(store);
    return 0;
}

int store_index_write(Store_t* store, StoreIndex_t* t)
{
    return pwrite(store->index_fd, t, sizeof(StoreIndex_t), t->self);
}

int store_delete(Store_t* store, int key)
{
    StoreIndex_t* index = (StoreIndex_t*)hashtable_get(store->hash_index, key);
    if (index == NULL) {
        return 1;
    }
    index_set_delete(index);
    store_index_write(store, index);
    return 0;
}

int store_put(Store_t* store, int key, const char* data, int n)
{
    // 1. write data to file 
    // 2. write index to index file
    // 3. put index to hash_index
    StoreIndex_t* index = (StoreIndex_t*)hashtable_get(store->hash_index, key);
    if (index == NULL) {
        index = (StoreIndex_t*)malloc(sizeof(StoreIndex_t));
        index->self = lseek(store->index_fd, 0, SEEK_END);
    }
    index->key = key;
    index->flag = 0;
    index->offset = lseek(store->fd, 0, SEEK_END);
    write(store->fd, &n, sizeof(int));
    write(store->fd, data, n);
    store_index_write(store, index);
    StoreIndex_t test;
    pread(store->index_fd, &test, sizeof(test), index->self);
    hashtable_put(store->hash_index, key, index);
    return 0;
}

int store_get(Store_t* store, int key, char **data, int *n)
{
    // 1. write data to file 
    // 2. write index to index file
    // 3. put index to hash_index
    StoreIndex_t* index = (StoreIndex_t*)hashtable_get(store->hash_index, key);
    if (index == NULL) {
        return 1;
    }
    off_t offset = index->offset;
    pread(store->fd, n, sizeof(int), offset);
    *data = (char*)malloc(*n);
    pread(store->fd, *data, *n, offset + sizeof(int));
    return 0;
}

void index_set_delete(StoreIndex_t *t)
{
    t->flag |= INDEX_FLAG_DELETE;
}
