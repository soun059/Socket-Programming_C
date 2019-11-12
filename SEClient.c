#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
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
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(atoi(argv[2]));
	server.sin_family = AF_INET;
	addrlen = sizeof(server);
	len = sizeof(message);
	message.syn = syn;
	message.ack = ack;
	message.flag = 2;
	len = sizeof(message);
	sendto(network_socket,&message,len,0,(struct sockaddr*)&server,addrlen);
	recvfrom(network_socket,&message2,len,0,(struct sockaddr*)&server,&addrlen);
	printf("Data sent from receiver syn:%d,ack:%d,flag:%d\n",message2.syn,message2.ack,message2.flag);
	if(message2.flag == 0)
	{
		syn = message2.ack;
		message.syn = syn;
		message.ack = ack;
		message.flag = 3;
		s_window_size = message.window_size = 30;
		sendto(network_socket,&message,len,0,(struct sockaddr*)&server,addrlen);
		recvfrom(network_socket,&message2,len,0,(struct sockaddr*)&server,&addrlen);
		printf("Data sent from receiver syn:%d,ack:%d,flag:%d\n",message2.syn,message2.ack,message2.flag);
		if(message2.syn == ack && message2.flag == 3)
		{
			printf("%s\n",message2.message);
			window_size = message2.window_size;
			while(1)
			{
				int size = 0;
				char mess[10];
				int SND = 0;
				while(size<window_size)
				{
					printf("Enter the message:");
					fgets(mess,10,stdin);
					size+=strlen(mess);
					strcat(message.message,mess);
					SND = rand()%2;
					if(size>=window_size || SND == 1)
					{
						ack += 1;
						syn = message2.ack;
						message.syn = syn;
						message.ack = ack;
						message.flag = 1;

						sendto(network_socket,&message,len,0,(struct sockaddr*)&server,addrlen);			
						recvfrom(network_socket,&message2,len,0,(struct sockaddr*)&server,&addrlen);
				printf("data sent from receiver syn:%d,ack:%d,flag:%d\n",message2.syn,message2.ack,message2.flag);
						break;
					}
					
				}
				
				
			}
		}
		else if(message2.syn != ack)
		{
			printf("Second level ack not same\n");
			return 0;
		}
		else
		{
			printf("%s\n",message2.message);
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