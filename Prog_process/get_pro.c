#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
//using getpid/getppid/getuid/geteuid/getgid/getegid

int main()
{
	
	pid_t pid = fork();

	if(pid < 0)
	{
		printf("%d\n",errno); //note that line braeks
		exit(2);
	}
	else if(pid == 0)
	{
		printf("child:pid->%d,ppid->%d,uid->%d,euid->%d,gid->%d,egid->%d\n",getpid(),getppid(),getuid(),geteuid(),getgid(),getegid());
		exit(0);
	}
	else
	{
		printf("father:pid->%d,ppid->%d,uid->%d,euid->%d,gid->%d,egid->%d\n",getpid(),getppid(),getuid(),geteuid(),getgid(),getegid());
		sleep(3);
	}
	return 0;
}
