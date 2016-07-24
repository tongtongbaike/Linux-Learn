#include<stdio.h>
#include<malloc.h>
#include<pthread.h>

typedef int _dataType;
typedef int *_dataType_p;
typedef struct _node{
	_dataType data;
	struct _node* next;
}node,*nodep,**nodepp;

nodep head = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

nodep buyNode(_dataType val)
{
	nodep tmp = (nodep)malloc(sizeof(node));
	if(tmp!= NULL)
	{
		tmp->data = val;
		tmp->next = NULL;
		return tmp;
	}	
	return NULL;
}

void init(nodepp head)
{
	*head = buyNode(0);
}
//存在头节点。
void push_list(nodep head,_dataType val)
{
	nodep tmp = buyNode(val);
	tmp->next = head->next;
	head->next = tmp;
}

int pop_back_list(nodep head,_dataType_p pval)
{
	//当之存在一个哨兵位的时候.
	if(NULL == head->next)
		return -1;
		
	nodep index = head;
	while(NULL != index->next->next)
	{
	     index = index->next;
	}
	nodep del = index->next;
	*pval = del->data;
	index->next = del->next;
	free(del);
	return 0;
}

void* product(void* arg)
{
	_dataType i = 0;
	while(1)
	{
		sleep(1);
		pthread_mutex_lock(&mutex);
		push_list(head,i++);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit((void *)1);
}

void* consumer(void *arg)
{
	_dataType val = 0;
	while(1){
		sleep(2);
		pthread_mutex_lock(&mutex);
		if(pop_back_list(head,&val) == -1)
		{
			pthread_mutex_unlock(&mutex);
			continue;
		}
		printf("data:%d\n",val);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit((void *)1);
}

int main()
{
	pthread_t tid1,tid2;
	init(&head);
	pthread_create(&tid1,NULL,product,NULL);
	pthread_create(&tid2,NULL,consumer,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	free(head);
	pthread_mutex_destroy(&mutex);
	return 0;
}
