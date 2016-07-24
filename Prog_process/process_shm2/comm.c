#include"comm.h"
static int _shm(size_t  size,int flags)
{
	key_t _key=ftok(_PATH_,_PROJ_ID_);
	if(_key<0)
	{
		perror("ftok");
		return -1;
	}
	int shm_id=shmget(_key,size,flags);
	if(shm_id<0)
	{
		perror("shmget");
		return -1;
	}
	return shm_id;
}
int creat_shm(size_t size)
{
	return _shm(size,IPC_CREAT|IPC_EXCL|0666);
}
int get_shm(size_t size)
{	
	return _shm(size,IPC_CREAT);
}
void* at_shm(int shm_id)
{
	return shmat(shm_id,NULL,0);
}
int dt_shm(const void* shmadd)
{
	return shmdt(shmadd);
}
int destory_shm(int shm_id)
{
	if(shmctl(shm_id,IPC_RMID,NULL)<0)
	{
		perror("shmctl");
		return -1;
	}
	return 0;
}
