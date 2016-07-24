#include<stdio.h>
#include<pthread.h>

int g_val = 0;

pthread_rwlock_t rw_lock;

void* reader(void* arg)
{
	//阻塞读写锁。
	while(1)
	{
		pthread_rwlock_rdlock(&rw_lock);
		printf("g_val: %d\n",g_val);
		pthread_rwlock_unlock(&rw_lock);	
	}
}

void* writer(void* arg)
{
	while(1)
	{
		sleep(1);
		pthread_rwlock_wrlock(&rw_lock);
		g_val++;
		pthread_rwlock_unlock(&rw_lock);
	}
}

int main()
{
	pthread_t tid1,tid2;
	pthread_rwlock_init(&rw_lock,NULL);
	pthread_create(&tid1,NULL,reader,NULL);
	pthread_create(&tid2,NULL,writer,NULL);
	pthread_rwlock_destroy(&rw_lock);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	return 0;
}
