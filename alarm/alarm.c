#include<stdio.h>
#include<unistd.h>

int main()
{
	int ret = 9;
	ret = alarm(10);
	printf("alarm 1:%d\n",ret);
	sleep(3);
	ret = alarm(10);
	printf("alarm 2:%d\n",ret);
	return 0;
}
