#include<stdio.h>
#include<pthread.h>

int g_val = 0;
pthread_rwlock_t rw_lock;

void* reader(void* arg)
{
	pthread_detach(pthread_self());
	//非阻塞读写锁。
	while(1)
	{
		if(pthread_rwlock_tryrdlock(&rw_lock)!= 0){
			printf("writer is wirting,reader waiting...\n");
		}
		else{
			printf("reader is:%u,g_val: %d\n",pthread_self(),g_val);
			pthread_rwlock_unlock(&rw_lock);
		}	
		sleep(2);
	}
}

void* writer(void* arg)
{	
	pthread_detach(pthread_self());
	while(1)
	{
		if(pthread_rwlock_trywrlock(&rw_lock) != 0){
			printf("reader is reading,writer waiting\n");
			sleep(1);
		}
		else{
			g_val++;
			printf("writer is :%u,writer val is :%d\n",pthread_self(),g_val);
			pthread_rwlock_unlock(&rw_lock);
		}
		sleep(1);
	}
}

int main()
{
	pthread_rwlock_init(&rw_lock,NULL);
	pthread_t id;
	int i ;
	for(i = 0;i<2;++i)
	{
		pthread_create(&id, NULL,writer,NULL);
	}
	for(i = 0;i < 3;++i)
	{
		pthread_create(&id,NULL,reader,NULL);
	}
	sleep(100);
	return 0;
}
