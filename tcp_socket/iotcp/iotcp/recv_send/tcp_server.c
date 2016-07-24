#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<signal.h>
#include<netinet/in.h>


#define _PORT_ 8888
#define _BACKLOG_ 2

void signal_proccess(int signo)
{
	printf("Catch exit siganl\n");
	exit(0);
}

void proccess_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[1024];
	while(1)
	{
		size = recv(s,buffer,1024,0);
		if(size == 0)
		{
			return ;
		}

		sprintf(buffer,"%d bytes altogether\n",size);

		send(s,buffer,strlen(buffer)+1,0);
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
