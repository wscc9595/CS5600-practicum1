#pragma once
#include <stdio.h>
#include "hashtable.h"

typedef struct
{
    int size;
    char data[];
} StoreData_t;

typedef struct
{
    int key;
    int flag;  // 0st bit mean delete_flag
    off_t self;
    off_t offset;
} StoreIndex_t;

/* 
 * simple store 
 * */
typedef struct
{
    int fd;
    int index_fd;
    HashTable_t* hash_index;
} Store_t;


enum INDEX_FLAG{
    INDEX_FLAG_DELETE = 0
};

Store_t* store_open(const char* s_path);

int store_close(Store_t*);

int store_delete(Store_t* store, int key);

int store_put(Store_t* store, int key, const char* data, int n);

int store_get(Store_t* store, int key, char **data, int *n);

int store_index_write(Store_t* store, StoreIndex_t *t);

void index_set_delete(StoreIndex_t *t);

