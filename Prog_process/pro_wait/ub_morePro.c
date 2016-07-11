#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<time.h>

//about how to wait process
//unblock_wait
//more fork();
//can build function check wait process

int child_run()
{
	srand(time(NULL));
	int _time = rand()%30;
	printf("bit,child pid is :%d,ppid is :%d time is :%d\n",getpid(),getppid(),_time);
	sleep(_time);
	return 0;
}

int creat_proc(pid_t *_pid_l,int num)
{
	if(_pid_l != NULL && num>0)
	{
		int i = 0;
		for(;i<num;++i)
		{
			printf("%d\n",num);
			pid_t id = fork();
			if(id <0)
			{
				printf("failed\n");
			}
			else if(id == 0)
			{
				int child_ret = child_run();
				printf("%dchild pid is %d ppid is: %d\n",i,getpid(),getppid());
				exit(1);
			}
			else
			{
				printf("i:%d,pid:%d,ppid:%d",i,getpid(),getppid());
				_pid_l[i] = id;
			}
		}
	}
	return 0;
}

int wait_proc(pid_t *_pid_l,int num)
{
	int _wait_ret = 0;
	if(_pid_l != NULL && num > 0)
	{
		int i;
		for(i = 0;i<num;i++)
		{
			if(_pid_l[i] == -1)
			{
				continue;
			}
			int status = 0;
			int ret = waitpid(_pid_l[i],&status,0);
			if(WIFEXITED(status) && ret == _pid_l[i])
				printf("wait child pid :%d success,code is :%d\n",_pid_l[i],WEXITSTATUS(status));
			else
			{
				printf("wait chile pid%d failed\n",_pid_l[i]);
				_wait_ret = 1;
			}
		}
	}
	return _wait_ret;
}

int main()
{
	pid_t _pid_list[10];
	int i = 0;
	for(;i < 10;++i)
	{
		_pid_list[i] = -1;
	}
	if(creat_proc(_pid_list,sizeof(_pid_list)/sizeof(_pid_list[0])) == 0)
	{
		printf("craete all process\n");
	}
	else
	{
		printf("create failed\n");
	}

	if(wait_proc(_pid_list,10) == 0)
	{
		printf("wait success\n");
	}
	else
	{
		printf("wait failed\n");
	}
	return 0;
			
}
