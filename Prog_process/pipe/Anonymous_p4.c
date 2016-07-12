#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

//read and write end side are not closed
//pipe was filed
//write block

int main()
{
	int _pipe[2];
	int ret = pipe(_pipe);
	if(ret  == -1)
	{
		printf("create pipe error");
		return -1;	
	}		
	pid_t id = fork();
	if(id < 0)
	{
		perror("fork");
		return 2;
	}
	else if(id == 0)
	{
		int i = 0;
		close(_pipe[0]);
		char* _mesg_c =NULL;
		while(1)
		{
			if(i < 1000)
			{	
		   		_mesg_c = "child";
				write(_pipe[1],_mesg_c,strlen(_mesg_c)+1);
			}
		++i;
		}
		close(_pipe[1]);
	}
	else{
		close(_pipe[1]);
		char _mesg[50];
		int j  = 0;
		while(j < 3)
		{
			sleep(10);	
			memset(_mesg,'\0',sizeof(_mesg));
			int ret = read(_pipe[0],_mesg,sizeof(_mesg));
			printf("%s:code is:%d\n",_mesg,ret);
			++j;
		}
		if(waitpid(id,NULL,0) < 0 )
		{
			return 3;
		}
	}
	return 0;
}
