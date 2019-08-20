#include<stdio.h>
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <errno.h>   
#include <unistd.h>  
#include <arpa/inet.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h>


int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("less data is provided\n");
		return 0;
	}
	int sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server;
	char buffer[1025];
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	printf("connecting to server\n");
	if(connect(sock,(struct sockaddr*)&server,sizeof(server))!=0)
	{
		printf("connection could not be made\n");
	}
	int n = recv(sock,&buffer,sizeof(buffer),0);
	buffer[n] = '\0';
	printf("message from server:%s\n",buffer);
	memset(buffer,'\0',sizeof('\0'));
	printf("-->");
	fgets(buffer,1024,stdin);
	send(sock,&buffer,sizeof(buffer),0);
	bzero(buffer,1024);
	recv(sock,&buffer,sizeof(buffer),0);
	printf("message from server:%s\n",buffer);
	close(sock);
	return 0;
}
