#ifndef PING_H
# define PING_H

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <math.h>
#include <getopt.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <argp.h>

#define ICMP_ECHO       8
#define ICMP_ECHOREPLY  0
#define ICMP_BODY_SIZE 56

typedef struct icmphdr icmphdr_t;

typedef struct s_stats
{
    struct timeval start_time;
    struct timeval send_time;

    int n_sent;
    int n_received;

    double min_rtt;
    double max_rtt;
    double total_rtt;
    double total_rtt_squared;
}               t_stats;

struct arguments
{
    char *target;
    int verbose;
};


// ping struct
typedef struct s_ping
{
    int sockfd;
    int socktype;

    struct arguments args;

    uint16_t seq;
    uint16_t pid;

    struct sockaddr_in addr;

    struct s_stats stats;
}               t_ping;

extern _Bool ping_running;

int send_echo_icmp(t_ping *ping);
int recv_echo_icmp(t_ping *ping);
int print_stats(t_ping *ping);
void print_usage(void);

#endif