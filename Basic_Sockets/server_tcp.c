#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("less number of arguments are given\n");
		return 0;
	}
	int network_socket,client_socket;
	network_socket = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server,client;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	if(bind(network_socket,(struct sockaddr*)&server,sizeof(server))<0)
	{
		printf("failed to bind\n");
		return 0;
	}
	listen(network_socket,5);
	while(1)
	{
		client_socket = accept(network_socket,NULL,NULL);
		int addrlen = sizeof(client);
		getpeername(client_socket,(struct sockaddr*)&client,&addrlen);
		printf("Received a connection from ip::%s,port::%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
		char buffer[1024];
		while(1)
		{
			memset(buffer,'\0',1024);
			printf("Enter a message for the client::");
			fgets(buffer,1024,stdin);
			int n = send(client_socket,buffer,1024,0);
			if(n>0)
				printf("Sent successfully\n");
			if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
			{
				close(client_socket);
				close(network_socket);
				return 0;
			}
			memset(buffer,'\0',1024);
			n = recv(client_socket,buffer,1024,0);
			if(n == 0)
				break;
			if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
			{
				break;
			}
			else if(n>0)
			{
				printf("Received data from client::%s\n",buffer);
			}
		}
		close(client_socket);
	}
}