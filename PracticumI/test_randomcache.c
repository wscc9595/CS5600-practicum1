#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "randomcache.h"

#include "test.h"

void test_rndcache()
{
    RandomCache_t* cache = rndcache_create();
    char* data;
    int len;
    data = NULL;
    if (rndcache_get(cache, 1, &data, &len) == 0)
    {
        ERROR("expect NULL get %s", data);
        return;
    }
    free(data);
    const char* teststr = "123";
    for (int i = 0; i < RNDCACHE_MAX_SIZE; i++)
    {
        rndcache_put(cache, i, teststr, strlen(teststr));
    }
    data = NULL;
    int ret;
    ret = rndcache_get(cache, 0, &data, &len);
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
    rndcache_put(cache, RNDCACHE_MAX_SIZE, teststr, strlen(teststr));
    int expired_cnt = 0;
    // some one will be expired
    for (int i = 0; i < RNDCACHE_MAX_SIZE; i++)
    {
        data = NULL;
        if (rndcache_get(cache, i, &data, &len) != 0)
        {
            expired_cnt += 1;
        }
        free(data);
    }
    if (expired_cnt != 1)
    {
        ERROR("expect 1 expired get (%d)", expired_cnt);
    }
    rndcache_destory(cache);
}

int main()
{
    test_rndcache();
    return 0;
}

