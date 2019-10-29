#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>

struct mess_t
{
    pthread_t t;
    char message[1024];
};

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 0;
    struct sockaddr_in server;
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket < 0)
    {
        printf("Could not create socket\n");
        return 0;
    }
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    server.sin_family = AF_INET;
    if (connect(network_socket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("could not connect to the server\n");
        return 0;
    }
    send(network_socket, "Hello Server", sizeof("Hello Server"), 0);
    struct mess_t mess;
    recv(network_socket, &mess, sizeof(mess), 0);
    printf("message received from server::::%s\n", mess.message);
    close(network_socket);
    return 0;
}