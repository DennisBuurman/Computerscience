/*
 * Skeleton-code behorende bij het college Netwerken, opleiding Informatica,
 * Universiteit Leiden.
 */

#ifndef asp_h
#define asp_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>

#include <math.h>

#include <sys/time.h>

/* An asp socket descriptor for information about the sockets current state */
typedef struct{
    int sockfd;

    struct sockaddr_in local_addr;
    socklen_t local_addrlen;

    struct sockaddr_in remote_addr;
    socklen_t remote_addrlen;

    int current_quality_level;
    int sequence_count;

    int packets_received;
    int packets_missing;
}asp_socket_info;

#endif
