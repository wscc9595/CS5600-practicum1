#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "message.h"

Message_t* create_msg(const char* sender, const char* receiver,
                      const char* content)
{
    // simple use a inc number as uid
    static uint64_t count = 0;
    ++count;
    Message_t* message = (Message_t*)malloc(sizeof(Message_t));
    message->flag = 0;
    message->uid = count;
    message->timestamp = time(NULL);
    message->content = strdup(content);
    message->receiver = strdup(receiver);
    message->sender = strdup(sender);
    return message;
}

Message_t* parse_msg(const char* data) {
    Message_t* message = (Message_t*)malloc(sizeof(Message_t));
    message->flag = *(int*)(data);
    data += sizeof(message->flag);
    message->uid = *(uint64_t*)(data);
    data += sizeof(message->uid);
    message->timestamp = *(uint64_t*)(data);
    data += sizeof(message->timestamp);
    int slen = strlen(data);
    message->sender = strdup(data);
    data += slen + 1;
    slen = strlen(data);
    message->receiver = strdup(data);
    data += slen + 1;
    slen = strlen(data);
    message->content = strdup(data);
    return message;
}

int serialize_msg(const Message_t* message, char** data, int* len)
{
    int bytelen = 0;
    bytelen = sizeof(message->flag) + sizeof(message->uid) +
              sizeof(message->timestamp);
    bytelen += strlen(message->sender) + 1;
    bytelen += strlen(message->receiver) + 1;
    bytelen += strlen(message->content) + 1;
    *data = calloc(1, bytelen);
    *len = bytelen;
    int sz = 0;
    memmove(*data + sz, &message->flag, sizeof(message->flag));
    sz += sizeof(message->flag);
    memmove(*data + sz, &message->uid, sizeof(message->uid));
    sz += sizeof(message->uid);
    memmove(*data + sz, &message->timestamp, sizeof(message->timestamp));
    sz += sizeof(message->timestamp);
    memmove(*data + sz, message->sender, strlen(message->sender));
    sz += strlen(message->sender) + 1;
    memmove(*data + sz, message->receiver, strlen(message->receiver));
    sz += strlen(message->receiver) + 1;
    memmove(*data + sz, message->content, strlen(message->content));
    sz += strlen(message->content);
    return 0;
}

int destory_msg(Message_t*msg)
{
    free(msg->content);
    free(msg->sender);
    free(msg->receiver);
    free(msg);
    return 0;
}

