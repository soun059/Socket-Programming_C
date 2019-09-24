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
    char name[1025];
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
    //printf("enter a name:");
    //scanf("%s",name);
	fd_set reads;
	printf("connecting to server\n");
	if(connect(sock,(struct sockaddr*)&server,sizeof(server))!=0)
	{
		printf("connection could not be made\n");
	}
	int n = recv(sock,&buffer,sizeof(buffer),0);
	buffer[n] = '\0';
	printf("message from server:%s\n",buffer);
	memset(buffer,'\0',sizeof('\0'));
	printf("Enter your name:");
	fgets(buffer,1021,stdin);
	strcat(buffer,"#$");
	send(sock,&buffer,sizeof(buffer),0);
	bzero(buffer,1024);
	//recv(sock,&buffer,sizeof(buffer),0);
	//printf("message from server:%s\n",buffer);
	while(1)
	{
		FD_ZERO(&reads);
		FD_SET(0,&reads);
		FD_SET(sock,&reads);
		int max_sd = sock;
		int activity = select(max_sd+1,&reads,NULL,NULL,NULL);
		if(activity<0 && errno!=EINTR)
			{
				printf("select error\n");
			}
		if(FD_ISSET(0,&reads)){
			fgets(buffer,1024,stdin);
			send(sock,&buffer,sizeof(buffer),0);
			if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
				{
					close(sock);
					exit(1);
				}
			bzero(buffer,1024);
			FD_CLR(0,&reads);
		}
		if(FD_ISSET(sock,&reads)){
			int valread = recv(sock,&buffer,sizeof(buffer),0);
			if(valread == 0)
				exit(1);
			printf("message from server:%s\n",buffer);
			if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
				{
					close(sock);
					exit(1);
				}
			bzero(buffer,1024);
			FD_CLR(sock,&reads);
		}
	}
	close(sock);
	return 0;
}
