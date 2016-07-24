#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<error.h>
#include<arpa/inet.h>

#define _PROT_ 8888
#define _BACKLOG_ 5

void process_conn_server(int s)
{
	ssize_t size =  0;
	char buffer[1024];
	while(1)
	{
		size = read(s,buffer,1024);

		if(size == 0)
		{
			return ;
		}

		sprintf(buffer,"%d bytes altongether\n",size);
		write(s,buffer,strlen(buffer) + 1);
		close(s);
	}
}

int main()
{
	int ss,sc;
	struct sockaddr_in server_sock;
	struct sockaddr_in client_sock;

	pid_t pid;
	int err;

	ss = socket(AF_INET,SOCK_STREAM,0);
	if(ss < 0)
	{
		printf("sock build error");
		return 1;
	}
	printf("lisock:%d\n",ss);
	bzero(&server_sock,sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sock.sin_port = htons(_PROT_);

	err = bind(ss,(struct sockaddr *)&server_sock,sizeof(server_sock));
	if(bind < 0)
	{
		printf("bind error");
		return 2;
	}

	err = listen(ss,_BACKLOG_);
	if(err < 0)
	{
		printf("listen is error");
		return 3;
	}


	while(1)
	{
		socklen_t len = sizeof(struct sockaddr);

		sc = accept(ss,(struct sockaddr *)&client_sock,&len);

		printf("sock:%d\n\n\n\n",sc);	

		if(sc < 0)
		{
			continue;
		}

		pid = fork();


		if(pid == 0)
		{
			process_conn_server(sc);
			close(ss);
		}
		else
		{
			close(sc);
		}

	}
	return 0;
}
