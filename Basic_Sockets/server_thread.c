#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>


pthread_mutex_t pt = PTHREAD_MUTEX_INITIALIZER;
int flag = 0;

void *myThread(void *arg)
{
	int client_socket = *((int*)arg);
	struct sockaddr_in client;
	int addrlen = sizeof(client);
	getpeername(client_socket,(struct sockaddr*)&client,&addrlen);
	char buffer[1024];
	printf("received connection from ip:%s,port:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	pthread_mutex_lock(&pt);
	while(1)
	{
		memset(buffer,'\0',1024);
		printf("Enter a message for the client:%ld::",pthread_self());
		fgets(buffer,1024,stdin);
		int n = send(client_socket,buffer,sizeof(buffer),0);
		if(n>0)
			printf("sent successfully\n");
		if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't' ){
			flag =1;
			break;
		}
		memset(buffer,'\0',1024);
		n = recv(client_socket,buffer,sizeof(buffer),0);
		if(n == 0 ||((buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't' )) )
			break;
		else if(n>0)
			printf("message from client:%ld:::%s\n",pthread_self(),buffer);
	}
	pthread_mutex_unlock(&pt);
	close(client_socket);
}

int main(int argc,char *argv[])
{
	int network_socket,client_socket;
	network_socket = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(atoi(argv[2]));
	server.sin_family = AF_INET;
	if(bind(network_socket,(struct sockaddr*)&server,sizeof(server))<0)
	{
		printf("could not bind\n");
		return 0;
	}
	pthread_t tid[10];
	int i = 0;
	listen(network_socket,5);
	while(1)
	{
		if(flag == 1)
			break;
		client_socket = accept(network_socket,NULL,NULL);
		if(pthread_create(&tid[i],NULL,myThread,&client_socket)!=0)
		{
			printf("failed to create a thread\n");
		}
		if(i>=10)
		{
			i=0;
			while(i<10)
				pthread_join(tid[i++],NULL);
			i=0;
		}
	}
	close(network_socket);
	return 0;
}