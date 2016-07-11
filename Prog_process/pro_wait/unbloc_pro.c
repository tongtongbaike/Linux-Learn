#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

//about how to wait process
//unblock_wait
//only have one fork();

int main()
{
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return 1;
	}
	else if(pid == 0)
	{
		printf("child is run,pid is %d\n",getpid());
		sleep(3);
		exit(255);
	}
	else
	{
		int status = 0;
		pid_t ret;
	   do{
			ret = waitpid(-1,&status,WNOHANG);
			if(ret == 0)
				printf("this wait\n");
			sleep(1);
	   }while(ret == 0);
		if(WIFEXITED(status) && ret == pid)
		{
			printf("child is running,code is %d\n",WEXITSTATUS(status));
		}
		else
		{
			printf("wait failed\n");
			return 1;
		}
	}
	return 0;
}

