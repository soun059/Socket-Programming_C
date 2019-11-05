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
	int network_socket;
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
	while(1)
	{
		if(buffer[0]=='l'&&buffer[1]=='s')
		{
			FILE *f;
			f = fopen("list1.txt","w");
			int file_size;
			recv(network_socket,&file_size,sizeof(file_size),0);
			char *file_buffer;
			file_buffer = (char*)malloc((file_size+1)*sizeof(char));
			recv(network_socket,file_buffer,file_size+1,0);
			fwrite(file_buffer,file_size,1,f);
			fclose(f);
			printf("\nReceived successfully\n");
		}
		else{
			memset(buffer,'\0',1024);
			int n = recv(network_socket,buffer,1024,0);
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
		memset(buffer,'\0',1024);
		printf("Enter a message for the server::");
		fgets(buffer,1024,stdin);
		int n = send(network_socket,buffer,1024,0);
		if(n>0)
				printf("Sent successfully\n");
		if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
		{
				break;
		}
	}
		close(network_socket);
}