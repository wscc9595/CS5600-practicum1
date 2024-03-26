#include <stdio.h>

#include "hashtable.h"

#include "test.h"

void test_hashtable()
{
    HashTable_t* t = hashtable_create();
    int a[100];
    for (int i = 0; i < 100; i++)
    {
        a[i] = i;
        hashtable_put(t, i, &a[i]);
    }
    if (hashtable_get(t, 100) != NULL)
    {
        ERROR("ERROR: expect NULL, get NOT NULL\n");
        return;
    }
    for (int i = 0; i < 100; i++)
    {
        int* value = hashtable_get(t, i);
        if (value != &a[i])
            ERROR("ERROR: expect %p, get %p\n", &a[i], value);
    }
    hashtable_delete(t, 0);
    if (hashtable_get(t, 0) != NULL)
    {
        ERROR("ERROR: expect NULL, get NOT NULL\n");
        return;
    }
    hashtable_destory(t);
}

int main()
{
    test_hashtable();
    return 0;
}

