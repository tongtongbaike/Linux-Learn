#include"semm.h"
   int main()
   {
       int sem_id=creat_sem_set(1);
       if(sem_id<0)
       {
           printf("error\n");
            return -1;
       }
      init_sem_set(sem_id,0,1);
      pid_t pid=fork();
      if(pid<0)
      {
          perror("pid");
          return -1;
      }
      else if(pid==0)
      {
          int sem_pid=get_sem_set(1);
          while(1)
          {
              p_sem_elem(sem_pid,0);
              printf("A");                                 
              sleep(1);
              fflush(stdout);
              printf("A");
              sleep(8);
              fflush(stdout);
              v_sem_elem(sem_pid,0);
          }
      }
      else
      {
          while(1)
          {
              p_sem_elem(sem_id,0);
              sleep(3);
              printf("B");
              sleep(2);
              fflush(stdout);
              printf("B");
                sleep(5);
              fflush(stdout);
              v_sem_elem(sem_id,0);
          }        
          waitpid(pid,NULL,0);
          destory(sem_id);
   
      }
      return 0;
  }
