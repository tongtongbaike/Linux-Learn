#pragma once
 #include<stdio.h>
 #include<stdlib.h>
 #include<string.h>
 #include<sys/types.h>
 #include<unistd.h>
 #include<sys/ipc.h>
 #include<sys/msg.h>
 #define _PATH_ "."
 #define _PROJ_ID_ 0x777
 #define _BLOCK_SIZE_ 1024
 #define _SERVER_MSG_TYPE_ 1
 #define _CLIENT_MSG_TYPE_ 2
 struct msgbuf
 {
     long mtype;
     char mtext[_BLOCK_SIZE_];
 };
 static int comm_msg_queue(int flag);
 int set_msg_queue();
 int get_msg_queue();
 int msg_queue_send(int msg_id,const char* msg,long type);
 int msg_queue_recv(int msg_id,char* msg,long type);
  int destory_msg_queue(int msgId);
