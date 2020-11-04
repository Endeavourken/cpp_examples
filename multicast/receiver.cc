#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct sockaddr_in group_addr;
    memset(&group_addr, 0, sizeof(group_addr));
    group_addr.sin_family = AF_INET;
    int ret = inet_pton(AF_INET, "224.1.0.1", &group_addr.sin_addr.s_addr);//all-hosts group
    if (1 != ret) {
        perror("Invalid internet address or address family");
        return -1;
    }
    group_addr.sin_port = htons(20003);

    int rcvSock = socket(AF_INET, SOCK_DGRAM, 0);
    const int on = 1;
    if (0 != setsockopt(rcvSock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
        perror("failed to set reuseaddr option on recv socket");
        return -1;
    }
    
    struct group_req group;
    group.gr_interface = 2;//for this host, it's ens33
    memset(&group.gr_group, 0, sizeof(group.gr_group));
    memcpy(&group.gr_group, &group_addr, sizeof(group_addr));

    if (0 != setsockopt(rcvSock, IPPROTO_IP, MCAST_JOIN_GROUP, &group, sizeof(group))) {
        perror("failed to set join multicast group on recv socket");
        return -1;
    }

    if (0 != bind(rcvSock, (const struct sockaddr*)&group_addr, sizeof(group_addr))) {
        perror("failed to bind");
        return -1;
    }

    char data[24];
    int i = 0;
    struct sockaddr_in source_addr;
    memset(&source_addr, 0, sizeof(source_addr));
    char source_ip[16];
    short source_port;
    socklen_t addr_len;
    while (true) {
        memset(data, 0, sizeof(data));
        addr_len = sizeof(source_addr);
        ret = recvfrom(rcvSock, data, sizeof(data), 0, (struct sockaddr*)&source_addr, &addr_len);
        if (ret == -1) {
            perror("Failed to receive");
            return -1;
        }
        memset(source_ip, 0, sizeof(source_ip));
        if (NULL == inet_ntop(AF_INET, &source_addr.sin_addr, source_ip, sizeof(source_ip))) {
            perror("Failed to parse sender's address");
            return -1;
        }
        source_port = ntohs(source_addr.sin_port);
        printf("Received %d bytes from %s:%d: %s\n", ret, source_ip, source_port, data);
        i++;
        
        if (i == 5) {
            struct group_source_req group_source;
            group_source.gsr_interface = 2;
            memcpy(&group_source.gsr_group, &group.gr_group, sizeof(group.gr_group));

            memset(&source_addr, 0, sizeof(source_addr));
            source_addr.sin_family = AF_INET;
            int ret = inet_pton(AF_INET, "192.168.31.162", &source_addr.sin_addr.s_addr);
            if (1 != ret) {
                perror("Invalid internet address or address family");
                return -1;
            }
            source_addr.sin_port = htons(20003);
            memset(&group_source.gsr_source, 0, sizeof(group_source.gsr_source));
            memcpy(&group_source.gsr_source, &source_addr, sizeof(source_addr));

            if (0 != setsockopt(rcvSock, IPPROTO_IP, MCAST_BLOCK_SOURCE, &group_source, sizeof(group_source))) {
                perror("failed to set join multicast group on recv socket");
                return -1;
            }
        }
    }
    return 0;
}
