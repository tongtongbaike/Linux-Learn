#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<error.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>


#define _PORT_ 8888

void process_conn_client(int s)
{
	ssize_t size  = 0;
	char buffer[1024];
	while(1)
	{
		size = read(0,buffer,1024);
		if(size > 0)
		{
			write(s,buffer,size);
			size = read(s,buffer,1024);
			write(1,buffer,size);
		}
	}
}

int main()
{
	int s;

	struct sockaddr_in server_sock;

	s = socket(AF_INET,SOCK_STREAM,0);
	if(s < 0)
	{
		printf("sock error");
		return 1;
	}

	bzero(&server_sock,sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sock.sin_port = htons(_PORT_);

	connect(s,(struct sockaddr *)&server_sock,sizeof(server_sock));
	
	printf("sock;%d\n",s);

	process_conn_client(s);

	close(s);

	return 0;
}
