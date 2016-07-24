#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define _SEM_PRO_ 20
#define _SEM_COM_ 0

typedef int _dataType;
_dataType blank[_SEM_PRO_];

sem_t sem_product;
sem_t sem_consumer;

void *product(void *arg)
{
	int index = 0;
	int count = 0;
	while(1)
	{
		sleep(rand()%5);
		sem_wait(&sem_product);
		blank[index++] = count ++;
		sem_post(&sem_consumer);
		index %= _SEM_PRO_;
	}
	pthread_exit((void *)1);
}

void* consumer(void * arg)
{
	int index = 0;
	while(1)
	{
		sem_wait(&sem_consumer);
		printf("data%d\n",blank[index++]);
		sem_post(&sem_product);
		index %= _SEM_PRO_;

	}
	pthread_exit((void*)1);
}

int main()
{
	pthread_t tid1,tid2;
	sem_init(&sem_product,0,20);
	sem_init(&sem_consumer,0,0);
	pthread_create(&tid1,NULL,product,NULL);
	pthread_create(&tid2,NULL,consumer,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	sem_destroy(&sem_product);
	sem_destroy(&sem_consumer);
	return 0;
}
