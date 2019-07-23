#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    int sock, length, fromlen, n;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buf[1024];
    fgets(buf, 1024, stdin);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        printf("failed to create a socket");
    }
    length = sizeof(server);
    bzero(&client, length);
    server.sin_family = AF_INET;
    struct hostent *hp;
    //server.sin_addr.s_addr = INADDR_ANY; // use inet_addr("127.0.0.1") instead of INADDR_ANY
    hp = gethostbyname("127.0.0.1");
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(5032);
    fromlen = sizeof(server);
    while (1)
    {
        n = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&server, fromlen);
        if (n < 0)
            error("sendto");
        bzero(buf, 1024);
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&server, &fromlen);
        if (n < 0)
            error("recvfrom");
        printf("Message received from server:%s\n", buf);
        fgets(buf, 1024, stdin);
    }
    return 0;
}