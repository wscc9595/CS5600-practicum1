#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "message.h"

#include "test.h"


void test_message()
{
    Message_t *msg = create_msg("me", "u", "hello");
    if(msg->flag != 0)
    {
        ERROR("ERROR: expect flag 0, get %d\n", msg->flag);
    }
    char *data;
    int len;
    if (serialize_msg(msg, &data, &len))
    {
        ERROR("ERROR: expect serialize_msg return 0 get not 0\n");
    }
    Message_t *newmsg = parse_msg(data);
    if (strcmp(newmsg->content, msg->content) != 0 || 
        strcmp(newmsg->sender, msg->sender) != 0 || 
        strcmp(newmsg->receiver, msg->receiver) != 0 ) 
    {
        ERROR("ERROR: expect msg same as newmsg get diff\n");
    }
    destory_msg(msg);
    destory_msg(newmsg);
    free(data);
}

int main()
{
    test_message();
    return 0;
}

