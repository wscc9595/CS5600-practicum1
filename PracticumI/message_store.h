#pragma once

#include "message.h"

typedef struct
{
    Store_t* store;

} MessageStore_t;

// save message in to store
int store_msg(MessageStore_t* ms_store, Message_t* message);

Message_t* retrieve_msg(MessageStore_t* ms_store, uint64_t uid);

MessageStore_t* open_message_store(const char * s_path);

int destory_message_store(MessageStore_t * ms_store);
