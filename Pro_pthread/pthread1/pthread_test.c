#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_t tid;
void* thread_run(void *_val)
{
	printf("%s,pid is :%d,tid is :%u\n",(char*)_val,(int )getpid(),(unsigned long long)pthread_self());
}

int main()
{
	int ret = pthread_create(&tid,NULL,thread_run,"other thread run");
	if(ret != 0)
	{
		printf("creater error:%s\n",strerror(ret));
		exit(ret);
	}

	printf("main thread run:pid is :%d,tid is :%u\n",(int )getpid(),(unsigned long long)pthread_self());
	sleep(1);
	return 0;
}

