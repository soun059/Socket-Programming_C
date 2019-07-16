#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main()
{
    char server_response[1024];
    FILE *http_response;
	http_response = fopen("fd.html","r");
    fgets(server_response,1024,http_response);
    char full_server_sent[2048] = "HTTP/1.1 200 OK\r\n\n";
    strcat(full_server_sent,server_response);
	printf("%s",full_server_sent);
    
    int server_socket;
    server_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9072);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket , (struct sockaddr*) &server_address , sizeof(server_address));
    listen(server_socket,5);
    int client_socket;
    while(1)
    {
        client_socket = accept(server_socket,NULL,NULL);
        send(client_socket,full_server_sent,sizeof(full_server_sent),0);
        close(client_socket);
    }
    return 0;

}
