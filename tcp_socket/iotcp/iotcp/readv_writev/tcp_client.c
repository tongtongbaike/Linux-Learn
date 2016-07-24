#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<signal.h>

static int s;

static struct iovec* vs = NULL;
static struct iovec* vc = NULL;

#define _PORT_ 8888

void sign_process(int signo)
{
	printf("Catch a exit signal\n");

	free(vc);

	_exit(0);
}

void sign_pipe(int signo)
{
	printf("Catch a SIGPIPE signal\n");
	
	free(vc);
	_exit(0);
}

void process_conn_client(int s)
{
	size_t size = 0;
	char buffer[30];

	struct iovec* v = (struct iovec*)malloc(3*sizeof(struct iovec));
	if(!v)
	{
		printf("create iovce is error");
		return ;
	}

	vc = v;

	v[0].iov_base = buffer;
	v[1].iov_base = buffer+10;
	v[2].iov_base = buffer+20;
	
	v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;

	int i = 0;
	while(1)
	{
		memset(buffer,'\0',sizeof(buffer));
		size = read(0,v[0].iov_base,10);
		if(size > 0)
		{
			v[0].iov_len = size;
			writev(s,v,1);
			v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;
			size = readv(s,v,3);
			for(i = 0;i < 3;++i)
			{
				if(v[i].iov_len >0 )
				{
					write(1,v[i].iov_base,v[i].iov_len);
				}
			}
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
