#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/time.h>
int network_socket,client_socket,pid;
int flag = 1;
void Child_Process(int client_socket,int pid)
{
	sleep(1);
	if(client_socket == 0)
		return ;
	struct sockaddr_in client;
	int addrlen = sizeof(client);
	getpeername(client_socket,(struct sockaddr*)&client,&addrlen);
	printf("Received connection from :: Ip:%s,Port:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	char buffer[1024];
	while(1)
	{
		memset(buffer,'\0',1024);
		printf("Enter something for the user_pid::%d ::",pid);
		fgets(buffer,1024,stdin);
		int n = send(client_socket,buffer,sizeof(buffer),0);
		if(n>0)
			printf("sent successfully\n");
		if (buffer[0] == 'e' && buffer[1] == 'i' && buffer[2] == 'i' && buffer[3] == 't')
		{
			flag = 0;
			close(client_socket);
			break;
		}
		memset(buffer,'\0',1024);
		n = recv(client_socket,&buffer,sizeof(buffer),0);
		if(n == 0)
		{
			close(client_socket);
			return ;
		}
		else if (buffer[0] == 'e' && buffer[1] == 'i' && buffer[2] == 'i' && buffer[3] == 't')
		{
			close(client_socket);
			return ;
		}
		else if(n>0)
		{
			printf("Received from client::%s\n",buffer);
		}
	}
}

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("less number of arguments are passed\n");
		return 0;
	}
	
	struct sockaddr_in server;
	network_socket = socket(AF_INET,SOCK_STREAM,0);
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	if(bind(network_socket,(struct sockaddr *)&server,sizeof(server))<0)
	{
		printf("could not bind\n");
		return 0;
	}
	listen(network_socket,5);
	while(1){
		client_socket = accept(network_socket,NULL,NULL);
		pid = fork();
		if(pid == 0)
		{
			Child_Process(client_socket,getpid());
		}
		if(flag == 0)
			break;
	}
	close(network_socket);
}	