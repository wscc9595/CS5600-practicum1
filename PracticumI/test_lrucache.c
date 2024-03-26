#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lrucache.h"

#include "test.h"

void test_lrucache()
{
    LRUCache_t* cache = lrucache_create();
    char* data;
    data = NULL;
    int len;
    if (lrucache_get(cache, 1, &data, &len) == 0)
    {
        ERROR("expect NULL get %s", data);
    }
    free(data);
    const char* teststr = "123";
    for (int i = 0; i < LRUCACHE_MAX_SIZE; i++)
    {
        lrucache_put(cache, i, teststr, strlen(teststr));
    }
    int ret;
    data = NULL;
    ret = lrucache_get(cache, 0, &data, &len);
    if (ret != 0)
    {
        ERROR("expect ret(0) size(%d) data(%s) get ret %d", len, teststr, ret);
    }
    else if (len != strlen(teststr) || strncmp(data, teststr, len) != 0)
    {
        ERROR("expect size(%lu) data(%s) get size(%d) data(%s)",
              strlen(teststr), teststr, len, data);
    }
    free(data);

    lrucache_put(cache, LRUCACHE_MAX_SIZE, teststr, strlen(teststr));
    data = NULL;
    // key(1) will be expired
    if (lrucache_get(cache, 1, &data, &len) == 0)
    {
        ERROR("expect NULL get %s", data);
        return;
    }
    free(data);
    for (int i = 0; i <= LRUCACHE_MAX_SIZE; i++)
    {
        if (i == 1) continue;
        data = NULL;
        ret = lrucache_get(cache, i, &data, &len);
        // printf("%d %d\n", i ,ret);
        if (ret != 0)
        {
            ERROR("expect ret(0) size(%d) data(%s) get ret %d", len, teststr,
                  ret);
        }
        else if (len != strlen(teststr) || strncmp(data, teststr, len) != 0)
        {
            ERROR("expect size(%lu) data(%s) get size(%d) data(%s)",
                  strlen(teststr), teststr, len, data);
        }
        free(data);
    }
    lrucache_destory(cache);
}

int main()
{
    test_lrucache();
    return 0;
}

