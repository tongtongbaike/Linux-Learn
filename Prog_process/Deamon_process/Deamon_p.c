#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>

void creat_deamon()
{
	int i ;
	int fd0;
	pid_t pid;
	struct sigaction sa;
	umask(0);
	if((pid = fork()) < 0)
	{
		printf("fork error");
	}
	else if(pid != 0)
	{
		exit(0);
	}

	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if(sigaction(SIGCHLD,&sa,NULL) < 0)
	{
		return ;
	}

	if((pid = fork()) < 0)
	{
		printf("fork error to child");
		return;
	}
	else if(pid != 0)
	{
		exit(0);
	}
	if(chdir("/") < 0)
	{
		printf("child dir error");
		return;
	}
	close(0);
	fd0 = open("dev/null",O_RDWR);
	dup2(fd0,1);
	dup2(fd0,2);
}

int main()
{
	creat_deamon();

	while(1)
	{
		sleep(1);
	}
	return 0;
}
