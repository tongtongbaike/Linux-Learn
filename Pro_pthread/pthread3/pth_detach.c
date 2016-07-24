#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_run(void *_val)
{
	pthread_detach(pthread_self());
	printf("%s\n",(char *)_val);
	return NULL;
}

int main()
{
	pthread_t tid;
	int tret = pthread_create(&tid,NULL,thread_run,"thread1 run...");
	if(tret != 0)
	{
		printf("create thread error,info is :%s\n",strerror(tret));	
		return tret;
	}
	int ret = 0;
	sleep(1);
	if(0 == pthread_join(tid,NULL))
	{
		printf("wait 1");
		ret = 0;
	}
	else{
		printf("wait failed");
		ret = 1;
	}
	return ret;
}
