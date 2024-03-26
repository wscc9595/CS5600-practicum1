#pragma once

#include <stdint.h>
#include "store.h"

typedef struct
{
    // message flag 
    int flag;
    // mesage unique identifier
    uint64_t uid;
    // message send time
    uint64_t timestamp;
    // message's sender receiver content dont contain '\0' 
    char* sender;
    char* receiver;
    char* content;
} Message_t;

Message_t* create_msg(const char* sender, const char* receiver,
                      const char* content);

// serialize message to bytes
int serialize_msg(const Message_t* message, char** data, int* len);

// parse data to message
Message_t* parse_msg(const char* data);

int destory_msg(Message_t*msg);

