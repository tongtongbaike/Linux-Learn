#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* thread1(void *_val)
{
	printf("thread 1 returning ...\n");
	return (void*)1;
}
void* thread2(void *_val)
{
	printf("thread 2 exiting ...\n");
	pthread_exit((void*)2);
}
void* thread3(void *_val)
{
	while(1)
	{
		printf("pthread 3is running,wait for be cancal...\n");
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	void *tret;
	//thread 1 return 
	pthread_create (&tid,NULL,thread1,NULL);
	pthread_join(tid,&tret);
	printf("thread return ,thread id is :%u,return code is : %d\n",(unsigned long long )tid,tret);

	//thread 2 return 
	void *tret2;
	pthread_create (&tid,NULL,thread2,NULL);
	pthread_join(tid,&tret2);
	printf("thread return ,thread id is :%u,exit code is : %d\n",(unsigned long long )tid,tret2);

	//thread 1 return 
	void *tret3;
	pthread_create (&tid,NULL,thread3,NULL);
	sleep(3);
	pthread_cancel(tid);
	pthread_join(tid,&tret3);
	printf("thread return ,thread id is :%u,cancel code is : %d\n",(unsigned long long )tid,tret3);
	return 0;
}
