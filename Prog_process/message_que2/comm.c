#include"comm.h"
static int comm_msg_queue(int flag)
{

    key_t _key=ftok(_PATH_,_PROJ_ID_);
    if(_key<0)
    {
        perror("ftok");
        return -1;
    }   
    int msg_id=msgget(_key,flag);
    if(msg_id<0)
    {
        perror("msgget");
        return -1;
    }   
    return msg_id;
}   
int set_msg_queue()
{
    umask(0);
    return comm_msg_queue(IPC_CREAT|IPC_EXCL|0666);
}   
int get_msg_queue()
{
    return comm_msg_queue(IPC_CREAT);
}   
int msg_queue_send(int msg_id,const char* message,long type)
{
    struct msgbuf msg;
    msg.mtype=type;
    strcpy(msg.mtext,message);
    if(msgsnd(msg_id,&msg,strlen(msg.mtext),0)<0)
    {
        perror("msgsnd");
        return -1;
    }   
    return 0;
 }
int msg_queue_recv(int msg_id,char* msg,long type)
{
    struct msgbuf ret;
    memset(ret.mtext,'\0',_BLOCK_SIZE_);
    if(msgrcv(msg_id,&ret,_BLOCK_SIZE_-1,type,0)<0)
    {
        perror("msgrcv");
        return -1;
    }
    strcpy(msg,ret.mtext);
    return 0;
}
int destory_msg_queue(int msg_id)
{
    if(msgctl(msg_id,IPC_RMID,NULL)<0)
    {
        perror("msgctl");
        return -1;
    }
    else
    {
        printf("remove msg_queue\n");
        return 0;
    }
}
