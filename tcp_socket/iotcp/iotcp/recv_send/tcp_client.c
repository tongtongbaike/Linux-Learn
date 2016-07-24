#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<signal.h>

static int s;

#define _PORT_ 8888

void sign_process(int signo)
{
	printf("Catch a exit signal\n");
	close(s);
	_exit(0);
}

void sign_pipe(int signo)
{
	printf("Catch a SIGPIPE signal\n");
}

void process_conn_client(int s)
{
	size_t size = 0;
	char buffer[1024];
	while(1)
	{
		size  =read(0,buffer,1024);

		if(size > 0)
		{
			send(s, buffer,size ,0);
			size = recv(s,buffer,1024,0);
			write(1,buffer,size);
		}
	}
}

int main(int argc,char *argv[])
{
	struct sockaddr_in server_sock;
	int err;
	if(1 == argc)
	{
		printf("PSL input server addr\n");
		return 0;
	}

	signal(SIGINT,sign_process);
	signal(SIGPIPE,sign_pipe);

	s = socket(AF_INET,SOCK_STREAM,0);
	if(s < 0)
	{
		printf("socket build error\n");
		return 1;
	}

	bzero(&server_sock,sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sock.sin_port = htons(_PORT_);

	inet_pton(AF_INET,argv[1],&server_sock.sin_addr);

	connect(s,(struct sockaddr *)&server_sock,sizeof(server_sock));

	process_conn_client(s);
	close(s);
	return 0;
}
