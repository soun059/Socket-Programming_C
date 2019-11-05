#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/time.h>
#include<errno.h>
#include<string.h>

int compare(char names[][5],char name[],int n)
{
	for(int i=0;i<n;i++)
	{
		int flag = 0;
		for(int j=0;j<5;j++)
		{
			if(names[i][j]!=name[j])
				flag = 1;
		}
		if(flag == 0)
			return i;
	}
	return -1;
}

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("less number of arguments are passed\n");
		return 0;
	}
	fd_set reads;
	int max_sd,network_socket,client_socket[30],len,n = 30,sd,valread,newsocket;
	for(int i=0;i<n;i++)
		client_socket[i] = 0;
	struct sockaddr_in server,client;
	len = sizeof(client);
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(atoi(argv[2]));
	server.sin_family = AF_INET;
	network_socket = socket(AF_INET,SOCK_STREAM,0);
	if(bind(network_socket,(struct sockaddr*)&server,sizeof(server))<0)
	{
		printf("bind failed\n");
		return 0;
	}
	listen(network_socket,30);
	char names[30][5];
	char *message = "Hello brother please enter your name in 5 characters followed by #$::";
	printf("waiting for the connections.......\n");
	char buff[1024];
	while(1)
	{
		FD_ZERO(&reads);
		FD_SET(0,&reads);
		FD_SET(network_socket,&reads);
		max_sd = network_socket;
		for(int i=0;i<n;i++)
		{
			sd = client_socket[i];
			if(sd>0)
				FD_SET(sd,&reads);
			if(sd>max_sd)
				max_sd = sd;
		}
		if(select(max_sd+1,&reads,NULL,NULL,NULL)<0 && errno!=EINTR)
		{
			printf("select error\n");
			continue;
		}
		if(FD_ISSET(network_socket,&reads))
		{
			if((newsocket = accept(network_socket,(struct sockaddr*)&client,&len))<0)
			{
				printf("could not accept the client\n");
				continue;
			}
			else
			{
				printf("Received connection from ip:%s,port:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
				if(send(newsocket,message,strlen(message),0)>0)
					printf("sent successfully\n");
				for(int i=0;i<n;i++)
				{
					if(client_socket[i] == 0){
						printf("acceptin the client into the client list\n");
						client_socket[i] = newsocket;
						break;
					}
				}
			}
		}
		if(FD_ISSET(0,&reads))
		{
			
			char buffer[1024];
			char name[5];
			fgets(buffer,1024,stdin);
			printf("Enter the name of the client::");
			fflush(stdin);
			fgets(name,6,stdin);
			int index = compare(names,name,n);
			if(index == -1)
			{
				printf("Name not found in the database...\n");
				
			}
			else{
				if(send(client_socket[index],buffer,strlen(buffer),0)>0)
					printf("sent successfully\n");;
			}
		}
		for(int i=0;i<n;i++)	
		{
			if(client_socket[i]!=0){
			sd = client_socket[i];
			if(FD_ISSET(sd,&reads))
			{
				memset(buff,'\0',1024);
				if((valread = recv(sd,buff,sizeof(buff),0)) == 0)
				{
					getpeername(sd,(struct sockaddr*)&client,&len);
					printf("host disconnected ip:%s,port:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port) );
					client_socket[i] = 0;
				}
				else
				{
					if(buff[5] =='#' && buff[6] == '$')
					{
						for(int j=0;j<5;j++)
							names[i][j] = buff[j];
						names[i][5] = '\0';
						printf("name successfully stored %s\n",names[i]);
					}
					else
						printf("message from %s::%s",names[i],buff);
					memset(buff,'\0',1024);
				}
			}
		}
	}
	}
}