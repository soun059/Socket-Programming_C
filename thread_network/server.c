#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

struct mess_t
{
    pthread_t t;
    char message[1024];
};

char message[1024];
char buffer[1024];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *newThread(void *arg)
{
    int newSocket = *((int *)arg);
    bzero(message, 1024);
    int p = recv(newSocket, &message, 1024, 0);
    pthread_mutex_lock(&lock); //locking the thread to call the newThread function and so no new thread can access it.
    struct mess_t mess;
    struct sockaddr_in client;
    int len = sizeof(client);
    getpeername(newSocket, (struct sockaddr *)&client, &len); //getting the peer name of the client currently connected to the server
    printf("message from ip:%s,port:%d:::::%s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), message);
    char mek[1024];
    printf("enter some message for client:%lu:::", pthread_self());
    fgets(mek, 1024, stdin);
    strcat(mess.message, mek);
    mess.t = pthread_self();
    send(newSocket, &mess, sizeof(mess), 0);
    pthread_mutex_unlock(&lock); //unlocking the thread code for other threads to access
    sleep(1);
    close(newSocket);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 0;
    int server_socket, newSocket;
    struct sockaddr_in server, client;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    int len = sizeof(server);
    bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    listen(server_socket, 50);
    pthread_t tid[30];
    int i = 0;
    while (1)
    {
        int addr_size = sizeof(client);
        newSocket = accept(server_socket, (struct sockaddr *)&client, &addr_size);
        if (pthread_create(&tid[i], NULL, newThread, &newSocket) != 0)
        {
            printf("Failed to create the thread");
        }
        if (i >= 50)
        {
            i = 0;
            while (i < 50)
            {
                pthread_join(tid[i++], NULL);
            }
            i = 0;
        }
    }
    close(server_socket);
    return 0;
}
