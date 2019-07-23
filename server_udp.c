#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>

int main()
{
    int sock, length, fromlen, n;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buf[1024];
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        printf("failed to create a socket");
    }
    length = sizeof(server);
    bzero(&server, length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // use inet_addr("127.0.0.1") instead of INADDR_ANY
    server.sin_port = htons(5032);
    if (bind(sock, (struct sockaddr *)&server, length) < 0)
        printf("\nerror binding");
    fromlen = sizeof(client);
    while (1)
    {
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&client, &fromlen);

        if (n < 0)
            error("recvfrom");
        printf("Message received from client:%s\n", buf);
        if (buf[0] == 'e' && buf[1] == 'x' && buf[2] == 'i' && buf[3] == 't')
            break;
        bzero(buf, 1024);
        fgets(buf, 1024, stdin);
        n = sendto(sock, buf, 1024, 0, (struct sockaddr *)&client, fromlen);
        if (buf[0] == 'e' && buf[1] == 'x' && buf[2] == 'i' && buf[3] == 't')
            break;
        if (n < 0)
            error("sendto");
        bzero(buf, 1024);
    }
    return 0;
}
