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
#include <pthread.h>

int check(char name[],char name1[])
{
	for(int i=0;i<strlen(name);i++)
	{
		//printf("%c:%c\n",name[i],name1[i]);
		if(name[i]=='\n' || name1[i]== ' ')
			return 1;
		if(name[i] != name1[i])
				return 0;
		
	}
	return 1;
}


int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("less data is provided\n");
		return 0;
	}
	int master_socket,len,new_socket,client_socket[30],max_client = 30,activity,sd,valread;
	int max_sd;
	char prev_buffer[1024];
	struct sockaddr_in address;
	char buffer[1025];
	fd_set reads;
	char *message = "hello from server";
	for(int i=0;i<max_client;i++)
		client_socket[i] = 0;
	if((master_socket = socket(AF_INET,SOCK_STREAM,0))==0)
	{
		printf("socket could not be created\n");
		return 0;
	}
	address.sin_addr.s_addr = inet_addr(argv[1]);
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[2]));
	if(bind(master_socket,(struct sockaddr*)&address,sizeof(address))<0)
	{
		printf("could not bind to the port\n");
		return 0;
	}
	printf("listening to port %d\n",atoi(argv[2]));
	if(listen(master_socket,6)<0)
	{
		printf("could not listen\n");
		return 0;
	}
	char names[30][1024];
	len = sizeof(address);
	printf("waiting for connections..........\n");
	while(1)
	{
		FD_ZERO(&reads);
		FD_SET(0,&reads);
		FD_SET(master_socket,&reads);
		max_sd = master_socket;
		for(int i=0;i<max_client;i++)
			{
				sd = client_socket[i];
				if(sd>0)
					FD_SET(sd,&reads);
				if(sd>max_sd)
					max_sd = sd;
			}
		printf("select called\n");
		activity = select(max_sd+1,&reads,NULL,NULL,NULL);
		if(activity<0 && errno!=EINTR)
			{
				printf("select error\n");
			}
		if(FD_ISSET(master_socket,&reads))
			{
				if((new_socket = accept(master_socket,(struct sockaddr*)&address,&len))<0)
					{
						printf("accept failure\n");
					}
            	printf("received connection from ip:%s port:%d\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
				if(send(new_socket,message,strlen(message),0)!=strlen(message))
					{
						printf("sending failure\n");
					}

					printf("message sent successfully\n");
					for(int i=0;i<max_client;i++)
					{
						if(client_socket[i]==0)
						{
							printf("accepting the client into the socket list %d\n",i);
							client_socket[i] = new_socket;
							break;
						}
					}
					//FD_CLR(master_socket,&reads);
		}
		for(int i=0;i<max_client;i++)
		{
			sd = client_socket[i];
			if(FD_ISSET(sd,&reads))
			{
				if((valread=recv(sd,buffer,sizeof(buffer),0)==0))
				{
					getpeername(sd,(struct sockaddr*)&address,&len);
					printf("host disconnected ip:%s port:%d\n",inet_ntoa(address.sin_addr),htons(address.sin_port) );
					client_socket[i] = 0;
					bzero(names[i],1024);
				}
				else
				{
					if(strcmp(buffer,"exit") == 0)
						exit(1);
					buffer[1023] = '\0';
					//printf("####{%c}:{%c}",buffer[strlen(buffer)-1],buffer[strlen(buffer)-2]);
					if(buffer[strlen(buffer)-1] == '$' && buffer[strlen(buffer)-2] == '#')
					{
						buffer[strlen(buffer)-3] = '\0';
						printf("message from host:%d :: my name is %s\n",i,buffer );
						strcpy(names[i],buffer); 
						strcpy(prev_buffer,buffer);
						/*printf("Enter message for the host %s-->",names[i]);
						bzero(buffer,1024);
						if(strcmp(buffer,"exit") == 0)
							exit(1);
						fgets(buffer,1024,stdin);
						send(sd,buffer,strlen(buffer),0);
						if(strcmp(buffer,"exit") == 0)
							exit(1);
						//FD_CLR(sd,&reads);*/
					}
					else
					{
						if(buffer[0] == '\0' || strcmp(prev_buffer,buffer) == 0)
							continue;
						printf("message from host:%s :: %s\n",names[i],buffer );
						strcpy(prev_buffer,buffer);
						if(strcmp(buffer,"exit") == 0)
						{
							getpeername(sd,(struct sockaddr*)&address,&len);
							printf("host disconnected ip:%s port:%d\n",inet_ntoa(address.sin_addr),htons(address.sin_port) );
							client_socket[i] = 0;
						}
						//FD_CLR(sd,&reads);
					}
					
				}
			}
		}
		if(FD_ISSET(0,&reads))
		{
						bzero(buffer,1024);
						fgets(buffer,1024,stdin);
						char name[1024];
						printf("Enter the name of the host -->");
						if(strcmp(buffer,"exit") == 0)
							exit(1);
						fgets(name,1024,stdin);
						for(int i=0;i<30;i++)
						{
							//printf("%d\n",check(name,names[i]));
							if(check(name,names[i]) && client_socket[i]!=0)
							{
								sd = client_socket[i];
								send(sd,buffer,strlen(buffer),0);
								break;
							}
						}
						if(strcmp(buffer,"exit") == 0)
							exit(1);
						FD_CLR(0,&reads);
						bzero(buffer,1024);
		}
	}
		return 0;
}
