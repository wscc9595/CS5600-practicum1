#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lrucache.h"
#include "message.h"
#include "message_store.h"
#include "randomcache.h"

void evaluation()
{
    srand(time(0));
    const int test_message_number = 200;
    const int test_access_number = 1000;
    uint64_t* uid = (uint64_t*)malloc(test_message_number * sizeof(uint64_t));
    MessageStore_t* msg_store = open_message_store("./_test_msg_store");
    LRUCache_t* lrucache = lrucache_create();
    RandomCache_t* randomcache = rndcache_create();
    for (int i = 0; i < test_message_number; i++)
    {
        // create a rand message
        static char content[11];
        for (int j = 0; j < 10; j++)
            content[j] = (rand() % 26) + 'a';
        content[10] = 0;
        Message_t* msg = create_msg("me", "u", content);
        uid[i] = msg->uid;
        store_msg(msg_store, msg);
        char* data;
        int len;
        serialize_msg(msg, &data, &len);
        lrucache_put(lrucache, msg->uid, data, len);
        rndcache_put(randomcache, msg->uid, data, len);
        free(data);
        destory_msg(msg);
    }

    {
        int lrumiss = 0;
        int lruhit = 0;
        for (int i = 0; i < test_access_number; i++)
        {
            uint64_t id = uid[rand() % test_message_number];
            char* data;
            int len;
            if (lrucache_get(lrucache, id, &data, &len) == 0)
            {
                lruhit += 1;
                free(data);
            }
            else
            {
                lrumiss += 1;
                Message_t* msg = retrieve_msg(msg_store, id);
                if (msg == NULL)
                {
                    fprintf(stderr, "ERROR get msg null id(%lu)", id);
                    return;
                }
                serialize_msg(msg, &data, &len);
                lrucache_put(lrucache, id, data, len);
                free(data);
                destory_msg(msg);
            }
        }
        printf("random access lrucache hit %d  miss %d hitratio: %f\n", lruhit,
               lrumiss, 1.0 * lruhit / (lruhit + lrumiss));
    }

    {
        const int frequent = 20;
        uint64_t* most = malloc(frequent * sizeof(uint64_t));
        for (int i = 0; i < 20; i++)
            most[i] = uid[rand() % test_message_number];
        int lrumiss = 0;
        int lruhit = 0;
        for (int i = 0; i < test_access_number; i++)
        {
            uint64_t id = uid[rand() % test_message_number];
            if (rand() % 3 == 0)
            {
                id = most[rand() % frequent];
            }
            char* data;
            int len;
            if (lrucache_get(lrucache, id, &data, &len) == 0)
            {
                lruhit += 1;
                free(data);
            }
            else
            {
                lrumiss += 1;
                Message_t* msg = retrieve_msg(msg_store, id);
                if (msg == NULL)
                {
                    fprintf(stderr, "ERROR get msg null id(%lu)", id);
                    return;
                }
                serialize_msg(msg, &data, &len);
                lrucache_put(lrucache, id, data, len);
                free(data);
                destory_msg(msg);
            }
        }
        printf("frequent access lrucache hit %d  miss %d hitratio: %f\n",
               lruhit, lrumiss, 1.0 * lruhit / (lruhit + lrumiss));
        free(most);
    }

    {
        int randommiss = 0;
        int randomhit = 0;
        for (int i = 0; i < test_access_number; i++)
        {
            uint64_t id = uid[rand() % test_message_number];
            char* data;
            int len;
            if (rndcache_get(randomcache, id, &data, &len) == 0)
            {
                randomhit += 1;
                free(data);
            }
            else
            {
                randommiss += 1;
                Message_t* msg = retrieve_msg(msg_store, id);
                if (msg == NULL)
                {
                    fprintf(stderr, "ERROR get msg null id(%lu)", id);
                    return;
                }
                serialize_msg(msg, &data, &len);
                rndcache_put(randomcache, id, data, len);
                free(data);
                destory_msg(msg);
            }
        }
        printf("randomcache hit %d  miss %d hitratio: %f\n", randomhit,
               randommiss, 1.0 * randomhit / (randomhit + randommiss));
    }

    {
        const int frequent = 20;
        uint64_t* most = malloc(frequent * sizeof(uint64_t));
        for (int i = 0; i < 20; i++)
            most[i] = uid[rand() % test_message_number];
        int randommiss = 0;
        int randomhit = 0;
        for (int i = 0; i < test_access_number; i++)
        {
            uint64_t id = uid[rand() % test_message_number];
            if (rand() % 3 == 0)
            {
                id = most[rand() % frequent];
            }
            char* data;
            int len;
            if (rndcache_get(randomcache, id, &data, &len) == 0)
            {
                randomhit += 1;
                free(data);
            }
            else
            {
                randommiss += 1;
                Message_t* msg = retrieve_msg(msg_store, id);
                if (msg == NULL)
                {
                    fprintf(stderr, "ERROR get msg null id(%lu)", id);
                    return;
                }
                serialize_msg(msg, &data, &len);
                rndcache_put(randomcache, id, data, len);
                free(data);
                destory_msg(msg);
            }
        }
        printf("frequent access randomcache hit %d  miss %d hitratio: %f\n",
               randomhit, randommiss,
               1.0 * randomhit / (randomhit + randommiss));
        free(most);
    }

    free(uid);
    lrucache_destory(lrucache);
    rndcache_destory(randomcache);
    destory_message_store(msg_store);
}

int main()
{
    evaluation();
    return 0;
}

