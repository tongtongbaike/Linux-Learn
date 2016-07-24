#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define _SEM_PRO_ 20
#define _SEM_COM_ 0

typedef int _dataType;
_dataType blank[_SEM_PRO_];

sem_t sem_product;
sem_t sem_consumer;
pthread_mutex_t mutex_product = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_consumer = PTHREAD_MUTEX_INITIALIZER;


void *product(void *arg)
{
	int index = 0;
	int count = 0;
	while(1)
	{
		sleep(rand()%5);
		sem_wait(&sem_product);
		pthread_mutex_lock(&mutex_product);
		printf("%d thread id doing\n",(int )arg);
		blank[index++] = count ++;
		index %= _SEM_PRO_;
		pthread_mutex_unlock(&mutex_product);
		sem_post(&sem_consumer);
	}
	pthread_exit((void *)1);
}

void* consumer(void * arg)
{
	int index = 0;
	while(1)
	{
		sem_wait(&sem_consumer);
		pthread_mutex_lock(&mutex_consumer);

		printf("%d thread is consumer ,data%d\n",(int)arg,blank[index++]);
		index %= _SEM_PRO_;
		pthread_mutex_unlock(&mutex_consumer);
		sem_post(&sem_product);
	}
	pthread_exit((void*)1);
}

int main()
{
	pthread_t tid1,tid2,tid3,tid4;
	sem_init(&sem_product,0,20);
	sem_init(&sem_consumer,0,0);
	pthread_create(&tid1,NULL,product,NULL);
	pthread_create(&tid1,NULL,product,NULL);
	pthread_create(&tid3,NULL,consumer,NULL);
	pthread_create(&tid4,NULL,consumer,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	pthread_join(tid4,NULL);
	sem_destroy(&sem_product);
	sem_destroy(&sem_consumer);
	pthread_mutex_destroy(&mutex_product);
	pthread_mutex_destroy(&mutex_consumer);
	return 0;
}
