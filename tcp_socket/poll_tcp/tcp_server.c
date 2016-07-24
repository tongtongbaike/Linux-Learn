#include<poll.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define _BACKLOG_ 5
#define _SIZE_ 64

static void usage (const char *proc)
{
	printf("%s [ip][prot]\n",proc);
}
 
static int start(char *ip,int port)	
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(1);
	}
	
	int opt = 1;
	if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(3);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	

	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind");
		exit(2);
	}

	if(listen(sock,_BACKLOG_) < 0)
	{
		perror("listen");
		exit(2);
	}
	return sock;
}



int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		return 1;
	}

	int port = atoi(argv[2]);
	char *ip = argv[1];

	int listen_sock = start(ip,port);

	//pollfd arrays
	struct pollfd polls[_SIZE_];
	int index = 0;
	int timeout = 5000; //check question time
	int i = 0;
	polls[0].fd = listen_sock;
	polls[0].events = POLLIN;
	polls[0].revents = 0;
	++index;
	//init polls.fd
	for(i = 1;i < _SIZE_;++i)
	{
		polls[i].fd = -1;
	}
	char buf [1024];
	ssize_t s = 0;
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	int done = 0;
	int max_fd = 1;
	while(!done)
	{
		switch(poll(polls,max_fd,timeout))
		{
			case -1:
				perror("poll");
				break;
			case 0:
				printf("timeout\n");
				break;
			default:
				{
					size_t i = 0;
					for(;i<_SIZE_;++i)
					{
						if((polls[i].fd == listen_sock) && (polls[i].revents == POLLIN))
						{
							int accept_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
							if(accept_sock < 0)
							{
								perror("accept");
								continue;
							}
							printf("connet success\n");
							for(;i < _SIZE_;++i)
							{
								if(polls[i].fd == -1)
								{
									polls[i].fd = accept_sock;
									polls[i].events = POLLIN;
									++max_fd;
									break;
								}
							}
							if(i == _SIZE_)
							{
								close(accept_sock);
							}
						}
						else if((polls[i].fd > 0) &&(polls[i].revents == POLLIN) )
						{
							ssize_t size = read(polls[i].fd,buf,sizeof(buf)-1);
							if(size < 0)
							{
								perror("read");
								continue;
							}
							else if(size == 0)
							{
								printf("client close \n");
								struct pollfd tmp = polls[i];
								polls[i] = polls[max_fd -1];
								polls[max_fd -1] = tmp;

								close(polls[max_fd - 1].fd);
								polls[max_fd - 1].fd = -1;
							}
							else
							{
								buf[size] = '\0';
								printf("client # %s",buf);
								polls[i].events = POLLOUT;
							}
						}
						else if(polls[i].fd > 0&& \
								polls[i].revents == POLLOUT)
						{
							write(polls[i].fd,buf,sizeof(buf));
							polls[i].events = POLLIN;
						}
					}
				}
				break;
		}
	}
	return 0;
}
