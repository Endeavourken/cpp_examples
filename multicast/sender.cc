#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int sndSock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    int ret = inet_pton(AF_INET, "192.168.31.162", &addr.sin_addr.s_addr);
    if (1 != ret) {
        perror("Invalid internet address or address family");
        return -1;
    }
    addr.sin_port = htons(20003);
    if (0 != bind(sndSock, (const struct sockaddr*)&addr, sizeof(addr))) {
        perror("failed to bind");
        return -1;
    }

    struct sockaddr_in group_addr;
    memset(&group_addr, 0, sizeof(group_addr));
    group_addr.sin_family = AF_INET;
    ret = inet_pton(AF_INET, "224.1.0.1", &group_addr.sin_addr.s_addr);//all-hosts group
    if (1 != ret) {
        perror("Invalid internet address or address family");
        return -1;
    }
    group_addr.sin_port = htons(20003);


    char greeting[] = "Hello guys";
    while (true) {
        ret = sendto(sndSock, greeting, sizeof(greeting), 0, (const struct sockaddr*)&group_addr, sizeof(group_addr));
        printf("Sent %d bytes\n", ret);
        system("sleep 2");
    }
    return 0;
}
