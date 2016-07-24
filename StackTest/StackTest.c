#include<stdio.h>
#include<stdlib.h>

void bug()
{
	system("reboot");
	exit(0);
}

int stack_test(int a,int b)
{
	printf("before writer : 0x%x\n",b);
	int *p = &a;
	p++;
	*p = 0xdddd;
	printf("after write : 0x%x\n",b);
	int c  = 0xcccc;
	return c;
}

int main()
{
	int a = 0xaaaa;
	int b = 0xbbbb;
	int ret = stack_test(a,b);
	printf("you should run here\n");
	return 0;
}
