#include <stdio.h>
#include <stdlib.h>

#include "message.h"
#include "message_store.h"
#include "store.h"

MessageStore_t* open_message_store(const char * s_path)
{
    MessageStore_t * ms_store = (MessageStore_t*)malloc(sizeof(MessageStore_t));
    ms_store->store = store_open(s_path);
    return ms_store;
}

int store_msg(MessageStore_t* ms_store, Message_t* message)
{
    char *data;
    int len;
    serialize_msg(message, &data, &len);
    store_put(ms_store->store, message->uid, data, len);
    free(data);
    return 0;
}

Message_t* retrieve_msg(MessageStore_t* ms_store, uint64_t uid)
{
    char *data;
    int len;
    if (store_get(ms_store->store, uid, &data, &len)) {
        return NULL;
    }
    Message_t * msg = parse_msg(data);
    free(data);
    return msg;
}

int destory_message_store(MessageStore_t * ms_store)
{
    store_close(ms_store->store);
    free(ms_store);
    return 0;
}
