#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main()
{
	int network_socket;
	network_socket = socket(AF_INET,SOCK_STREAM,0);
	

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	int connection_status = connect(network_socket, (struct sockaddr*) &server_address , sizeof(server_address));
	if (connection_status == -1){
		printf("there was an error in the connection");
		return 0;
	}
	char server_response[256];
	recv(network_socket,&server_response,sizeof(server_response),0);
	printf("response from server:%s\n",server_response);
	int y = 1;
	while(y==1){
		printf("-->");
		scanf("%s",server_response);
		send(network_socket,&server_response,sizeof(server_response),0);
		if(server_response == "bye")
			break;
		recv(network_socket,&server_response,sizeof(server_response),0);
		printf("Server:%s\n",server_response);
		if(server_response == "bye")
			break;
		
	}
	close(network_socket);
	return 0;
}
	
