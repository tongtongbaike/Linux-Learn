#include"comm.h"
int main()
{
    int msgid=set_msg_queue();
    if(msgid<0)
    {
        exit(1);
    }
    char buf[_BLOCK_SIZE_];
    printf("input quit endding..\n");
    while(1)
    {
        if(msg_queue_recv(msgid,buf,_CLIENT_MSG_TYPE_)<0)
        {
            printf("recv fail\n");
            exit(1);
        }
        else
        {
            if(strcmp("quit",buf)==0)
                return 0;
            printf("client:%s\n",buf);
        }
        printf("input:");
        fflush(stdout);
        memset(buf,'\0',_BLOCK_SIZE_);
        gets(buf);
        if(msg_queue_send(msgid,buf,_SERVER_MSG_TYPE_)<0)
        {
            printf("send fail\n");
            exit(1);
        }
    }
    destroy(msgid);
    return 0;
}
