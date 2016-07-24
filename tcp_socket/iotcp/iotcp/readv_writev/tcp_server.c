#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<signal.h>
#include<netinet/in.h>
#include<sys/uio.h>

static struct iovec* vs = NULL;
static struct iovec* vc = NULL;


#define _PORT_ 8888
#define _BACKLOG_ 2

void signal_proccess(int signo)
{
	printf("Catch exit siganl\n");
	free(vs);
	exit(0);
}

void proccess_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[30];

	struct iovec* v = (struct iovec*)malloc(3*sizeof(struct iovec));
	if(!v)
	{
		printf("creat iovec is failed\n");
		return;
	}
	vs = v;
	v[0].iov_base = buffer;
	v[1].iov_base = buffer+10;
	v[2].iov_base = buffer+20;
	v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;


	while(1)
	{
		size = readv(s,v,3);
		if(size == 0)
		{
			return ;
		}

		sprintf(v[0].iov_base,"%d",size);
		sprintf(v[1].iov_base,"bytes alt");
		sprintf(v[2].iov_base,"ogther\n");

		v[0].iov_len = strlen(v[0].iov_base);
		v[1].iov_len = strlen(v[1].iov_base);
		v[2].iov_len = strlen(v[2].iov_base);
		writev(s,v,3);
	}
}

int main()
{
	int ss,sc;
	struct sockaddr_in server_sock;
	struct sockaddr_in client_sock;
	int err;
	
	pid_t pid;
	signal(SIGINT,signal_proccess);
	signal(SIGPIPE,signal_proccess);

	ss = socket(AF_INET,SOCK_STREAM,0);
	if(ss < 0)
	{
		printf("socket build is error");
		return 1;
	}

	bzero(&server_sock,sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sock.sin_port = htons(_PORT_);

	err = bind(ss,(struct sockaddr*)&server_sock,sizeof(server_sock));
	if(err <  0)
	{
		printf("bind is error");
		return 2;
	}

	err = listen(ss,_BACKLOG_);
	if(err < 0)
	{
		printf("build listen is error");
		return 3;
	}

	while(1)
	{
		int addr = sizeof(struct sockaddr);

		sc = accept(ss,(struct sockaddr*)&client_sock,&addr);

		if(sc < 0)
		{
			printf("accept build is error");
			return 3;
		}

		pid = fork();
		if(pid == 0)
		{
			close(ss);
			proccess_conn_server(sc);
		}
		else{
			close(sc);
		}
	}
	return 0;
}
