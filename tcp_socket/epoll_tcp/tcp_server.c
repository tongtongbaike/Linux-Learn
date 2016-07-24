#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/epoll.h>

#define _BACKLOG_ 5
#define _MAX_FD_NUM_ 64


typedef struct data_buf
{
	int fd;
	char buf[1024];
}data_buf_t,*data_buf_p;

void usage(char *porc)
{
	printf("%s: [ip][port]\n",porc);
}

static int set_non_block(int fd)
{
	int old_fl = fcntl(fd,F_GETFL);
	if(old_fl < 0)
	{
		perror("fcntl");
		return -1;
	}
	if(fcntl(fd,F_SETFL,old_fl |O_NONBLOCK))
	{
		perror("fcntl");
		return -1;
	}
	return 0;
}

int startup(char *ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);

	int opt = 1;
	if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)))
	{
		perror("setsockopt");
		exit(1);
	}

	struct sockaddr_in listen_sock;
	listen_sock.sin_family = AF_INET;
	listen_sock.sin_port = htons(port);
	if(strcmp(ip,"any") == 0)
		listen_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	else 
		listen_sock.sin_addr.s_addr = inet_addr(ip);

	if(bind(sock,(struct sockaddr*)&listen_sock,sizeof(listen_sock)) < 0)
	{
		perror("bind");
		exit(2);
	}

	if(listen(sock,_BACKLOG_) < 0)
	{
		perror("listen");
		exit(3);
	}

	return sock;
}
int read_data(int fd,char *buf,int size)
{
	assert(buf);
	int ret = -1;
	int index = 0;
	printf("join read_data\n");
	while(index < size)
	{
		printf("join read_data while\n");
		ret = read(fd,buf+index,size-index);
		printf("ret:%d,read success,fd is %d\n",ret,fd);
		if(ret > 0)
		{			
			index += ret;
		}
		else
		{
			if(errno == EAGAIN)
			{
				printf("EAGAIN\n");
				return index;
			}
			perror("read");
			printf("now fd is %d",fd);
			return index;
		}
	}
	return index;
}

static int epoll_server(int sock)
{
	int epoll_fd = epoll_create(256);
	if(epoll_fd < 0)
	{
		perror("epoll_create");
		exit(4);
	}
	
	printf("epoll_create success\n");
	struct epoll_event ev;
	ev.events = EPOLLIN |EPOLLET;
	ev.data.fd = sock;
	
	if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sock,&ev) < 0)
	{
		perror("epoll_ctl");
		exit(5);
	}

	//设置一个输出的参数数组;
	struct epoll_event ev_out[_MAX_FD_NUM_];
	
	int max = _MAX_FD_NUM_;
	int timeout = 5000;
	int num = -1;
	int i = 0;
	int done = 0;
	data_buf_p mem = (data_buf_p)malloc(sizeof(data_buf_t));
	while(!done)
	{
		 //switch(num = epoll_wait(epoll_fd,ev_out,max,timeout))
		
		num = epoll_wait(epoll_fd,ev_out,max,timeout);
		printf("num is %d\n",num);
		switch(num)
		{
			case 0://timeout
				printf("timeout \n");
				break;
			case -1:
				perror("epoll_wait");
				break;
			default:
				{
					for(i = 0; i < num;++i)
					{
						if((ev_out[i].data.fd == sock) && (ev_out[i].events & (EPOLLIN | EPOLLET)))
						{
							struct sockaddr_in client;
							socklen_t len = sizeof(client);

							int fd = ev_out[i].data.fd;
							int newsock = accept(fd,(struct sockaddr*)&client,&len);
							if(newsock < 0)
							{
								perror("newsock");
								continue;	
							}
							int err = set_non_block(newsock);
							if(err <  0)
							{
								printf("non_block error\n");
								close(newsock);
								continue;
							}
							ev.events = EPOLLIN | EPOLLET;
							ev.data.fd = newsock;
							epoll_ctl(epoll_fd,EPOLL_CTL_ADD,newsock,&ev);
							printf("get a new connect\n");

						}
						else if(ev_out[i].events & (EPOLLIN |EPOLLET))
						{
							printf("join read\n");
							int fd = ev_out[i].data.fd;
						//	data_buf_p mem = (data_buf_p)malloc(sizeof(data_buf_t));
							if(!mem)
							{
								perror("malloc");
								continue;
							}
							
							mem->fd = fd;
							ssize_t _s = read_data(mem->fd,mem->buf,sizeof(mem->buf)-1);
							 // ssize_t _s = read_data(mem->fd,mem->buf,sizeof(mem->buf) - 1);
							if(_s > 0)
							{
								mem->buf[_s] = '\0';
								printf("%d client:%s,len:%d\n",mem->fd,mem->buf,_s);
								ev.events = EPOLLOUT | EPOLLET;
								ev.data.ptr = mem;
								epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&ev);
								printf("change fd success EPOLLOUT\n");
							}
							else if(_s == 0)
							{
								printf("client close...\n");
								epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,NULL);
								close(fd);
								free(mem);
							}
							else
							{
								printf("data_read is failed");
								continue;
							}
						}
						else if (ev_out[i].events & (EPOLLOUT |EPOLLET))
						{
							data_buf_p mem = (data_buf_p)ev_out[i].data.ptr;
							int fd = mem->fd;
							char *buf = mem->buf;
							write(fd,buf,strlen(buf));
							ev.events = EPOLLIN | EPOLLET;
							ev.data.ptr = mem->buf;
							ev.data.fd = mem->fd;
							epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&ev);
							printf("echo write success,change fd EPOLLIN\n");
						}
						else
						{
								
						}
					}
				}
				break;
		}
	}
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		return -1;
	}

	int port = atoi(argv[2]);
	char *ip = argv[1];
	int listen_sock = startup(ip,port);
	printf("listen succed\n");
	epoll_server(listen_sock);
	close(listen_sock);

	return 0;
}


