#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "message_store.h"

#include "store.h"
#include "test.h"


void test_msg_store()
{
    MessageStore_t* ms_store = open_message_store("./_test_msg_store");
    Message_t *msg = create_msg("me", "u", "hello");
    store_msg(ms_store, msg);
    Message_t *newmsg = retrieve_msg(ms_store, msg->uid);
    if (newmsg == NULL || strcmp(newmsg->content, msg->content) != 0 || 
        strcmp(newmsg->sender, msg->sender) != 0 || 
        strcmp(newmsg->receiver, msg->receiver) != 0 ) 
    {
        ERROR("ERROR: expect msg same as newmsg get diff\n");
    }
    destory_msg(msg);
    destory_msg(newmsg);
    destory_message_store(ms_store);
}   

int main()
{
    test_msg_store();
    return 0;
}

