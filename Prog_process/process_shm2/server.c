#include"comm.h"
int main()
{
	int shm_id=creat_shm(_SIZE_);
	char* buf=(char*)at_shm(shm_id);
	//memset(buf,'\0',_SIZE_);
	while(1)
	{
		printf("%s\n",buf);
		sleep(1);
	}
	dt_shm(buf);
	destory(buf);
}
