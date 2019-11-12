#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<string.h>

//client is data sender



struct mess_t
{
	int syn;
	int ack;
	int window_size;
	char message[30];
	int flag; // 2 for syn, 3 for ack, 0 for syn+ack, 1 for data
};

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("less arguments are passed\n");
		return 0;
	}
	int network_socket,syn = 0,ack = syn+1,len,addrlen,s_window_size,window_size;
	struct mess_t message, message2;
	network_socket = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in client,server;
	client.sin_addr.s_addr = inet_addr(argv[1]);
	client.sin_port = htons(atoi(argv[2]));
	client.sin_family = AF_INET;
	if(bind(network_socket,(struct sockaddr*)&client,sizeof(client))<0)
	{
		printf("bind failed\n");
		return 0;
	}
	addrlen = sizeof(server);
	len = sizeof(message);
	message.syn = syn;
	message.ack = ack;
	message.flag = 0;
	len = sizeof(message);
	recvfrom(network_socket,&message2,len,0,(struct sockaddr*)&server,&addrlen);
	printf("Data sent from sender syn:%d,ack:%d,flag:%d\n",message2.syn,message2.ack,message2.flag);
	if(message2.flag == 2)
	{
		syn = message2.ack;
		message.syn = syn;
		message.ack = ack;
		message.flag = 0;
		s_window_size = message.window_size = 30;
		sendto(network_socket,&message,len,0,(struct sockaddr*)&server,addrlen);
		recvfrom(network_socket,&message2,len,0,(struct sockaddr*)&server,&addrlen);
		printf("Data sent from sender syn:%d,ack:%d,flag:%d\n",message2.syn,message2.ack,message2.flag);
		if(message2.syn == ack && message2.flag == 3)
		{
			syn = message2.ack;
			message.syn = syn;
			message.ack = ack;
			message.flag = 3;
			s_window_size = message.window_size = 30;
			window_size = message2.window_size;
			strcat(message.message,"Connection established");
			sendto(network_socket,&message,len,0,(struct sockaddr*)&server,addrlen);
			while(1)
			{
				memset(message2.message,'\0',30);
				recvfrom(network_socket,&message2,len,0,(struct sockaddr*)&server,&addrlen);
				printf("data sent from sender syn:%d,ack:%d,flag:%d,message:%s\n",message2.syn,message2.ack,message2.flag,message2.message);
				ack += strlen(message.message);
				syn = message2.ack;
				message.syn = syn;
				message.ack = ack;
				message.flag = 3;
				message.window_size = s_window_size;
				sendto(network_socket,&message,len,0,(struct sockaddr*)&server,addrlen);			
			}
		}
		else if(message2.syn != ack)
		{
			printf("Second level ack not same\n");
			return 0;
		}
		else
		{
			message.syn = syn;
			message.ack = ack;
			message.flag = 0;
			s_window_size = message.window_size = 30;
			strcat(message.message,"Connection failed");
			sendto(network_socket,&message,len,0,(struct sockaddr*)&server,addrlen);	
			printf("Second level Flag is not same\n");
			return 0;
		}

	}
	else
	{
		printf("Flag is not same \n");
		return 0;
	}
	return 0;
}