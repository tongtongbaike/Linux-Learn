#include"semm.h"

static int sem_create_sem_set(int _sem_nums,int flag)
{
	printf("joint frok\n");
	key_t _key = ftok(_KEY_PATH,PROJECT_ID);
	if(_key < 0){
		printf("ftok failed\n");
		perror("ftok");
		return -1;
	}
	int sem_id = semget(_key,_sem_nums,flag);
	if(sem_id < 0)
	{
		printf("get failed\n");
		perror("semget");
		return -1;
	}
	printf("sem_id:%d\n",sem_id);
	printf("create success\n");
	return sem_id;
}
int creat_sem_set(int _semnums)
{
	int _flag = IPC_CREAT|IPC_EXCL|0666;
	int ret = sem_create_sem_set(_semnums,_flag);

	printf("ret:sem_id: %d\n",ret);
	return ret;
}
int get_sem_set(int _sem_nums)
{
	int _flag = IPC_CREAT;
	return sem_create_sem_set(_sem_nums,_flag);
}
int init_sem_set(int _sem_id,int _seq_num,int _init_val)
{
	union semun _sem;
	_sem.val = _init_val;
	int ret = semctl(_sem_id,_seq_num,SETVAL,_sem);
	printf("join init set\n");
	if(ret == -1)
	{
		perror("semctl");
		printf("HHHHH");
		return ret;
	}
	return ret;
}
int p_sem_elem(int _sem_id,int seq_num)
{
	struct sembuf _sem_buf[1];
	_sem_buf[0].sem_num = seq_num;
	_sem_buf[0].sem_op = -1;
	_sem_buf[0].sem_flg = 0;
	return semop(_sem_id,_sem_buf,1);
}

int v_sem_elem(int _sem_id,int seq_num)
{
	struct sembuf _sem_buf[1];
	_sem_buf[0].sem_num = seq_num;
	_sem_buf[0].sem_op = 1;
	_sem_buf[0].sem_flg = 0;
	return semop(_sem_id,_sem_buf,1);
}


int destory(int _sem_id)
{
	int ret = semctl(_sem_id,IPC_RMID,0,NULL);
	if(ret == -1)
	{
		perror("semctl");
		return ret;
	}
	return ret;
}
