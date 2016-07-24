#include"comm.h"
int main()
{
	int shm_id=get_shm(_SIZE_);
	if(shm_id<0)
	{
		printf("get_shm error\n");
		return -1;
	}
	char* buf=(char*)at_shm(shm_id);
	memset(buf,'\0',_SIZE_);
	int i=0;
	while(i<_SIZE_-1)
	{
		sleep(1);
		buf[i++]='A';
	}
	dt_shm(buf);
	return 0;
}
