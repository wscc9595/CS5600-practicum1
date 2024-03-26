#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "store.h"

#include "test.h"

void test_store()
{
    const char* db_path = "./_test_store.db";
    const char* teststr = "123";
    const char* teststr2 = "4567";
    char* data;
    int ret;
    {
        Store_t* store = store_open(db_path);
        if (store == NULL)
        {
            ERROR("expect store not NULL get NULL");
            return;
        }
        int len = strlen(teststr);
        store_put(store, 1, teststr, len);
        int size;
        data = NULL;
        ret = store_get(store, 1, &data, &size);
        if (ret != 0)
        {
            ERROR("expect ret(0) size(%d) data(%s) get ret %d", len, teststr,
                  ret);
        }
        else if (size != len || strncmp(data, teststr, len) != 0)
        {
            ERROR("expect size(%d) data(%s) get size(%d) data(%s)", len,
                  teststr, size, data);
        }
        free(data);
        
        data = NULL;
        ret = store_get(store, 2, &data, &size);
        if (ret == 0)
        {
            ERROR("expect data NULL get %p", data);
        }
        free(data);

        store_put(store, 2, teststr, len);
        data = NULL;
        ret = store_get(store, 2, &data, &size);
        if (ret != 0)
        {
            ERROR("expect ret(0) size(%d) data(%s) get ret %d", len, teststr,
                  ret);
        }
        else if (size != len || strncmp(data, teststr, len) != 0)
        {
            ERROR("expect size(%d) data(%s) get size(%d) data(%s)", len,
                  teststr, size, data);
        }
        free(data);

        ret = store_get(store, 1, &data, &size);
        if (ret != 0)
        {
            ERROR("expect ret(0) size(%d) data(%s) get ret %d", len, teststr,
                  ret);
        }
        else if (size != len || strncmp(data, teststr, len) != 0)
        {
            ERROR("expect size(%d) data(%s) get size(%d) data(%s)", len,
                  teststr, size, data);
        }
        free(data);

        len = strlen(teststr2);

        store_put(store, 2, teststr2, len);
        data = NULL;
        ret = store_get(store, 2, &data, &size);
        if (ret != 0)
        {
            ERROR("expect ret(0) size(%d) data(%s) get ret %d", len, teststr,
                  ret);
        }
        else if (size != len || strncmp(data, teststr2, len) != 0)
        {
            ERROR("expect size(%d) data(%s) get size(%d) data(%s)", len,
                  teststr2, size, data);
        }
        free(data);

        store_close(store);
    }
    // test reopen
    {
        Store_t* store = store_open(db_path);
        if (store == NULL)
        {
            ERROR("expect store not NULL get NULL");
            return;
        }
        int size, len;
        data = NULL;
        ret = store_get(store, 2, &data, &size);
        len = strlen(teststr2);
        if (ret != 0)
        {
            ERROR("expect ret(0) size(%d) data(%s) get ret %d", len, teststr,
                  ret);
        }
        else if (size != len || strncmp(data, teststr2, len) != 0)
        {
            ERROR("expect size(%d) data(%s) get size(%d) data(%s)", len,
                  teststr2, size, data);
        }
        free(data);

        data = NULL;
        ret = store_get(store, 1, &data, &size);
        len = strlen(teststr);
        if (ret != 0)
        {
            ERROR("expect ret(0) size(%d) data(%s) get ret %d", len, teststr,
                  ret);
        }
        else if (size != len || strncmp(data, teststr, len) != 0)
        {
            ERROR("expect size(%d) data(%s) get size(%d) data(%s)", len,
                  teststr, size, data);
        }
        free(data);
        store_close(store);
    }
}

int main()
{
    test_store();
    return 0;
}

