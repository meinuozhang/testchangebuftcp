#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#define BUF_SIZE 512
int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("usage:ip port senbuf\n",basename(argv[0]));
		return 1;
	}
	const char* ip=argv[1];
	int port=atoi(argv[2]);
    int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in remote;
	remote.sin_family=AF_INET;
	remote.sin_port=htons(port);
	remote.sin_addr.s_addr=inet_addr(ip);

	int sendbuf=atoi(argv[3]);
	int len1=sizeof(sendbuf);
	setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sendbuf,len1);
	getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sendbuf,(socklen_t*)&len1);
	printf("the top sendbuf size is %d\n",sendbuf);

    if(connect(sock,(struct sockaddr*)&remote,sizeof(remote))>0)
	{
		char buf[BUF_SIZE];
		memset(buf,'\0',sizeof(buf));
		send(sock,buf,BUF_SIZE,0);
	}
	close(sock);
	return 0;
}

