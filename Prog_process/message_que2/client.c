#include"comm.h"
  int main()
  {
      int msgid=get_msg_queue();
      if(msgid<0)
      {
          exit(1);
      }
      char buf[_BLOCK_SIZE_];
      while(1)
      {
          fflush(stdout);
          printf("please input:");
          memset(buf,'\0',_BLOCK_SIZE_);
          gets(buf);
          if(msg_queue_send(msgid,buf,_CLIENT_MSG_TYPE_)<0)
          {
              printf("send fail\n");
              exit(1);
          }
          if(msg_queue_recv(msgid,buf,_SERVER_MSG_TYPE_)<0)
          {
              printf("recv fail\n");
              exit(1);
          }
          printf("server:%s\n",buf);
      }
      return 0;
  }
