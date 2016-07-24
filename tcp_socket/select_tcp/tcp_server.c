#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<error.h>
#include<assert.h>

#define _BACKLOG_ 5

int fds[64]; //文件描述符集合

//命令行参数出错
static void usage(const char *proc)
{
	printf("usage : %s [ip][port]\n",proc);
}

//服务器端TCP的链接绑定监听状态。
static int startup(char *ip,int port)
{
	assert(ip);
	
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	int set = -1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(set)) < 0)
	{   
	   	perror("setsoickopt");
		exit(4);
	}

	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
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


int main(int argc,char *argv[])
{
	//判断命令参数行.
	if(argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}

	int port = atoi(argv[2]);
	char *ip = argv[1];

	int listen_sock = startup(ip,port);
	int done = 0;
	int new_sock = -1;
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	
	int max_fd; //selet函数的第一个记录参数/
	
	//读写事件文件描述符
	fd_set reads;
	fd_set writes;

	int i = 0;
	int fds_num = sizeof(fds)/sizeof(fds[0]);
	
	//对文件描述符集合进行初始化
	for(;i < fds_num;++i)
	{
		fds[i] = -1;
	}

	//当前存在listen文件描述符。
	fds[0] = listen_sock;
	
	max_fd = fds[0];	//初次调用select函数设置max_fd的值；

	while(!done)
	{
		//初始化读写文件描述符
		FD_ZERO(&reads);
		FD_ZERO(&writes);
		//将listen_sock设置为reads，因为时在等待请求相应，相当与当前的读取操作。
		FD_SET(listen_sock,&reads);
		//设置多路复用中的轮寻时间表。
		struct timeval timeout = {5,0};
		
		//没次读取添加事件到reads中。;
		for(i = 1;i < fds_num; ++i)
		{
			if(fds[i] >0)
			{
				FD_SET(fds[i],&reads);
				if(fds[i] > max_fd)
				{
					max_fd = fds[i];
				}
			}
		}

		switch(select(max_fd+1, &reads,&writes,NULL,&timeout))
		{
			case 0 ://timeout
				{
					printf("select timeout\n");
					break;
				}

			case -1:
				{	//error	
					perror("select");
					break;
				}

			default:
				{	//返回改变了的文件描述.
					i = 0;
					char buf[1024];
					//遍历所有的文件描述符集合。
					for(;i<fds_num;++i)
					{
						//确认是否时监听时间,是的话就绪要accept；
						if(fds[i] == listen_sock && \
								FD_ISSET(fds[i],&reads))
						{
							new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
							if(new_sock <0)
							{
								perror("accept");
								continue;
							}
							printf("get a new connet...%d\n",new_sock);
							for(i = 0;i< fds_num;++i)
							{
								if(fds[i] == -1)
								{
									fds[i] = new_sock;
									break;
								}
							}
							if(i == fds_num)
							{
								close(new_sock);
							}
						}
		
						else if(fds[i] > 0 &&\
								FD_ISSET(fds[i],&reads))	//正常事件，但是是非监听时间，也就代表时新建立的new_sock。
								{
								//	char buf[1024];
									ssize_t s = read(fds[i],buf,sizeof(buf) -1);
									if(s > 0)
									{
										buf[s] = '\0';
									//	printf("client : %s\n",buf);
										printf("client : %s",buf);
										FD_SET(fds[i],&writes);
									//	write(fds[i],buf,sizeof(s)+1);
									}
									else if(s == 0)
									{
										printf("client quit...\n");
										close(fds[i]);
										fds[i] = -1;
									}
									else{}
								}
						else{}

						if(fds[i] > 0&&\
								FD_ISSET(fds[i],&writes))
						{
							write(fds[i],buf,sizeof(buf));
						}
					}
				}
		break;
		}
	}
	return 0;
}

