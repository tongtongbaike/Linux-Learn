#include "shm.h"

int get_shm()
{
	key_t key = ftok(_PATH_,_PROJECT_);
	
	if(key < 0)
	{
		perror("ftok");
		return -1;
	}

	int flag =IPC_EXCL| IPC_CREAT |0666;
	int shm_id = shmget(key,_SHM_SIZE_,flag);
	if(shm_id < 0)
	{
		printf("shmid error");
	}
	else
	{
		printf("shmid success");
	}
	return shm_id;
}     

void* at_shm(int shm_id)
{
	return shmat(shm_id,NULL,0);
}

int delete_shm(char *addr)
{
	return shmdt(addr);
}

int rm_shmctl(int shm_id)
{
	return shmctl(shm_id,IPC_RMID,NULL);
}


