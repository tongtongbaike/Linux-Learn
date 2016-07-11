#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>


int main()
{
	pid_t pid;
	int i = 0;
	for(;i<3;++i)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork");
			return 1;
		}
		else if(pid == 0)
		{
			printf("%dchild pid is %d ppid is: %d\n",i,getpid(),getppid());
		}
		else
		{
			printf("%dfather pid is %d ppid is: %d\n",i,getpid(),getppid());
		}
	}
	return 0;
}

