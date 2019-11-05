#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include<netdb.h>
#include <sys/types.h>
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Less arguments are passed\n");
        return 0;
    }
    int network_socket;
    struct sockaddr_in server, client;
    network_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    if (bind(network_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Bind failed\n");
        return 0;
    }
    char buffer[1024];
    while (1)
    {
        memset(buffer, '\0', 1024);
        int len = sizeof(client);
        int n = recvfrom(network_socket, buffer, 1024, 0, (struct sockaddr *)&client, &len);
        if (n > 0)
        {
            printf("received client connection from ip::%s and port::%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            printf("Message from client:::%s", buffer);
            printf("Enter a message for the client::");
            fgets(buffer, 1024, stdin);
            int n = sendto(network_socket, buffer, 1024, 0, (struct sockaddr *)&client, sizeof(client));
            if (n > 0)
                printf("Sent successfully\n");
        }
    }
}