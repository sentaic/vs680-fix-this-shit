// SPDX-License-Identifier: GPL-2.0-only
/* UDP 转接 CBOR v2，按来源地址和端口维持单会话。 */
#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#define LIMIT 12488
static uint64_t now_ms(void) {
    struct timespec now;clock_gettime(CLOCK_MONOTONIC,&now);
    return (uint64_t)now.tv_sec*1000+(uint64_t)now.tv_nsec/1000000;
}
/* Canonical CBOR v2 maps start with version (key 0), then operation (key 1). */
static int operation(const uint8_t *packet, ssize_t count) {
    if(count<5 || packet[0]<0xa2 || packet[0]>0xac ||
       packet[1]!=0 || packet[2]!=2 || packet[3]!=1)return -1;
    if(packet[4]<24)return packet[4];
    if(packet[4]==24 && count>=6)return packet[5];
    return -1;
}
int main(int argc, char **argv) {
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: %s BIND_ADDRESS PEER_ADDRESS [PORT]\n", argv[0]);
        return 2;
    }
    struct in_addr bind_ip, peer_ip;
    if (inet_pton(AF_INET, argv[1], &bind_ip) != 1 ||
        inet_pton(AF_INET, argv[2], &peer_ip) != 1) {
        fputs("Expected IPv4 addresses\n", stderr);
        return 2;
    }
    char *end = NULL;
    long port = argc == 4 ? strtol(argv[3], &end, 10) : 5681;
    if (port < 1 || port > 65535 || (argc == 4 && (end == argv[3] || *end))) {
        fputs("Port must be in 1..65535\n", stderr);
        return 2;
    }
    int udp=socket(AF_INET,SOCK_DGRAM|SOCK_CLOEXEC,0), local=-1;
    struct sockaddr_in bind_addr={.sin_family=AF_INET,.sin_port=htons((uint16_t)port),.sin_addr=bind_ip}, peer={0};
    if(udp<0 || bind(udp,(void*)&bind_addr,sizeof bind_addr)) { perror("UDP bind");return 1; }
    uint64_t last_rx=0;
    printf("CBOR v2 UDP %s:%ld ready\n", argv[1], port);fflush(stdout);
    for(;;) {
        uint64_t now=now_ms();
        if(local>=0 && now-last_rx>=2000) {close(local);local=-1;}
        struct pollfd waiters[2]={{udp,POLLIN,0},{local,POLLIN,0}};
        int ready=poll(waiters,2,local>=0?(int)(2000-(now-last_rx)):-1);
        if(ready<0) { if(errno==EINTR)continue;perror("poll");return 1; }
        if(local>=0 && waiters[1].revents&(POLLHUP|POLLERR|POLLNVAL)) {close(local);local=-1;waiters[1].fd=-1;waiters[1].revents=0;}
        if(waiters[0].revents&POLLIN) {
            uint8_t packet[LIMIT+1]; struct sockaddr_in sender; socklen_t size=sizeof sender;
            ssize_t count=recvfrom(udp,packet,sizeof packet,0,(void*)&sender,&size);
            if(count<=0 || count>LIMIT || sender.sin_addr.s_addr!=peer_ip.s_addr)continue;
            int op=operation(packet,count);
            if(op==1 && local>=0) {close(local);local=-1;waiters[1].fd=-1;waiters[1].revents=0;}
            if(local<0) {
                struct sockaddr_un addr={.sun_family=AF_UNIX};strcpy(addr.sun_path,"/run/vs680-hid/control.sock");
                local=socket(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC|SOCK_NONBLOCK,0);
                if(local<0 || connect(local,(void*)&addr,sizeof addr)) {if(local>=0)close(local);local=-1;continue;}
                peer=sender;
            }
            if(sender.sin_addr.s_addr!=peer.sin_addr.s_addr || sender.sin_port!=peer.sin_port)continue;
            last_rx=now_ms();
            ssize_t sent=send(local,packet,(size_t)count,MSG_NOSIGNAL|MSG_DONTWAIT);
            if(sent!=count) {
                int error=errno;perror("local send");
                if(error!=EAGAIN && error!=EWOULDBLOCK && error!=EINTR) {close(local);local=-1;}
            }
            if(op==29 && local>=0) {close(local);local=-1;waiters[1].fd=-1;waiters[1].revents=0;}
        }
        if(local>=0 && waiters[1].fd==local && waiters[1].revents&POLLIN) {
            uint8_t reply[LIMIT];
            ssize_t count=recv(local,reply,LIMIT,MSG_TRUNC|MSG_DONTWAIT);
            if(count<=0 || count>LIMIT) {close(local);local=-1;continue;}
            if(sendto(udp,reply,(size_t)count,MSG_DONTWAIT,(void*)&peer,sizeof peer)!=count)perror("UDP reply");
        }
    }
}
