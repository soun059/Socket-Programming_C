#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("less number of arguments are given\n");
		return 0;
	}
	int network_socket;
	fd_set reads;
	network_socket = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	char buffer[1024];
	if(connect(network_socket,(struct sockaddr*)&server,sizeof(server))<0)
	{
		printf("failed to connect\n");
		return 0;
	}	
	recv(network_socket,buffer,sizeof(buffer),0);
	printf("%s",buffer);
	char name[8];
	fgets(name,8,stdin);
	send(network_socket,name,strlen(name),0);
	while(1)
	{
		FD_ZERO(&reads);
		FD_SET(0,&reads);
		FD_SET(network_socket,&reads);
		
		if(FD_ISSET(0,&reads))
		{
			memset(buffer,'\0',1024);
			fgets(buffer,1024,stdin);
			if(send(network_socket,buffer,strlen(buffer),0)>0)
				printf("sent successfully\n");;
		}
		if(FD_ISSET(network_socket,&reads))
			{
				int valread;
				memset(buffer,'\0',1024);
				if((valread = recv(network_socket,buffer,sizeof(buffer),0)) == 0)
				{
					printf("server disconnected\n");
					break;
				}
				else
						printf("message from server::%s",buffer);
			}
	}
		close(network_socket);
}