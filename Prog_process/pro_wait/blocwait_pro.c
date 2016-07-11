#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

//about how to wait process
//block_wait

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
		pid_t ret = waitpid(-1,&status,0); //wait() similar this function
		printf("this wait\n");
		if(WIFEXITED(status) && ret == pid)
		{
			printf("wait success,code is %d\n",WEXITSTATUS(status));
		}
		else
		{
			printf("wait failed\n");
			return 1;
		}
	}
	return 0;
}

