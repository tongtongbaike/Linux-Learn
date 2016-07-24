#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NLOOP 5000

static int g_count = 0;

void* read_write_mem(void *_val)
{
	int val = 0;
	int i = 0;
	for (;i<NLOOP;++i)
	{
		val = g_count ;
		printf("pthread id is :%x,count is :%d\n",(unsigned long )pthread_self(),g_count);
		g_count = val +1;
	}
	return NULL;
}

int main()
{
	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1,NULL,read_write_mem,NULL);
	pthread_create(&tid2,NULL,read_write_mem,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	printf("count final val is :%d\n",g_count);
	return 0;
}
