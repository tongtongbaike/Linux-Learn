#include "shm.h"

int main()
{
	int shm_id = get_shm();
	if(shm_id < 0)
	{
		printf("HHH");
	}
	pid_t id = fork();
	if(id <0)
	{
		printf("fork error\n");
		return 1;
	}
	else if(id == 0)
	{
		char *buf = (char *)at_shm(shm_id);

		printf("child");
		int i = 0;
		while(i < 4095)
		{
			buf[i] = 'A';
			++i;
			buf[i] = '\0';
			sleep(1);
		}
		buf[4095] = '\0';
		delete_shm(buf);
	}
	else
	{
		int i = 0;
		char *buf  = (char *)at_shm(shm_id);
		printf("father");
		while(i<4095)
		{
			printf("%s\n",buf);
			++i;
			sleep(1);
		}
		delete_shm(buf);
		//waitpid(id,NULL,0);
		rm_shmctl(shm_id);
	}
	return 0;

}
