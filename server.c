#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#define BUF_SIZE 1024
int startup(int port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		exit(1);
	}
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
        perror("bind");
		exit(2);
	}
	if(listen(sock,5)<0)
	{
		perror("listen");
		exit(3);
	}
	return sock;
}
int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("usage:%s ip port recvbuf\n",basename(argv[0]));
		return 1;
	}
	const char* ip=argv[1];
	int port=atoi(argv[2]);
	int listen_sock=startup(port);
    int recvbuf=atoi(argv[3]);
    int len1=sizeof(recvbuf);
	setsockopt(listen_sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,sizeof(recvbuf));
	getsockopt(listen_sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,(socklen_t*)&len1);
	printf("the tcp receive buf size after setting is %d\n",recvbuf);
     
	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	
	 int new_sock=accept(listen_sock,(struct sockaddr*)&client,&len);
	 if(new_sock<0)
	 {
		 perror("accept");
		 exit(1);
	 }
     else
	 {
		 char buf[BUF_SIZE];
         memset(buf,'\0',sizeof(buf));
		 while(recv(new_sock,buf,BUF_SIZE-1,0))
		 {
			 close(new_sock);
		 }
	 }
	 close(listen_sock);
	return 0;
}
