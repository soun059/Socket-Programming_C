#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>

void listall()
{
	system("ls>>list.txt");
}

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
	server.sin_port = htons(atoi(argv[2]));
	server.sin_family = AF_INET;
	if(bind(network_socket,(struct sockaddr*)&server,sizeof(server))<0)
	{
		printf("failed to bind\n");
		return 0;
	}
	listen(network_socket,6);
	while(1)
	{
		client_socket = accept(network_socket,NULL,NULL);
		int addrlen = sizeof(client);
		getpeername(client_socket,(struct sockaddr*)&client,&addrlen);
		printf("Received a connection from ip:%s,port:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
		char buffer[1024];
		while(1)
		{
			memset(buffer,'\0',1024);
			printf("Enter a message for the client::");
			fgets(buffer,1024,stdin);
			int n = send(client_socket,buffer,sizeof(buffer),0);
			if(n>0)
				printf("sent successfully\n");
			if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't' )
			{
				close(client_socket);
				close(network_socket);
				return 0;
			}
			memset(buffer,'\0',1024);	
			n = recv(client_socket,buffer,sizeof(buffer),0);
			if(n == 0 || (buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't' ))
			{
				close(client_socket);
				break;
			}
			else if(buffer[0] == 'l' && buffer[1] == 's') 
			{
				listall();
				FILE *f;
				f = fopen("list.txt","rb");
				char *file_buffer;
				fseek(f,0l,SEEK_END);
				int file_len = ftell(f);
				printf("file_Size:%d",file_len);
				send(client_socket,&file_len,sizeof(file_len),0);
				rewind(f);
				file_buffer = (char*)malloc((file_len+1)*sizeof(char));
				fread(file_buffer,file_len,1,f);
				printf("contents of file_buffer:%s\n",file_buffer);
				int n = send(client_socket,file_buffer,file_len+1,0);
				if(n>0)	
					printf("sent successfully\n");
				fclose(f);

			}
			else if(n>0)
			{
				printf("message from the client::%s\n",buffer);
			}
		}
	}
	return 0;
}