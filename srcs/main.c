#include "ping.h"

_Bool ping_running = 1;

void handler(int signum)
{
	if (signum == SIGINT)
		ping_running = 0;
}

// **
// ** Print usage information

void print_usage(void)
{
    puts(
        "Usage\n"
        "  ft_ping [options] <destination>\n"
        "\n"
        "Options:\n"
        "  <destination>      DNS name or IP address\n"
        "  -h                 print help and exit\n"
        "  -v                 verbose output\n"
        "\n"
    );
    exit(2);
}

// **
// ** Parse command line arguments
// ** we need to handle -v and -? options
int parse_args(t_ping *ping, int ac, char **av)
{
    int opt;

    while ((opt = getopt(ac, av, "vh?")) != EOF)
    {
        switch (opt)
        {
            case 'v':
                ping->verbose = 1;
                break;
            case 'h':
            case '?':
                print_usage();
            default:
                print_usage();
        }
    }

    ac -= optind;
    av += optind;

    if (ac == 0)
        print_usage();

    return (0);
}


int resolve_host(t_ping *ping)
{
    struct addrinfo hints = {0};
    struct addrinfo *res;
    int err;

    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    err = getaddrinfo(ping->target, NULL, &hints, &res);
    // ! close fd
    if (err != 0)
    {
        printf("ping: %s: %s\n", ping->target, gai_strerror(err));
        return (-1);
    }

    memcpy(&ping->addr, res->ai_addr, sizeof(struct sockaddr_in));
    freeaddrinfo(res);
    return (0);
}


// **
// ** Initialize raw socket
int init_socket(t_ping *ping)
{
    // create raw socket
    ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping->sockfd < 0)
    {
        perror("socket");
        return (-1);
    }
    return (0);
}

int print_stats(t_ping *ping)
{
    // print the statistics
    struct timeval end_time, diff;
	gettimeofday(&end_time, NULL);
	timersub(&end_time, &ping->stats.start_time, &diff);
	long total_ms = diff.tv_sec * 1000 + diff.tv_usec / 1000;

    printf("\n--- %s ping statistics ---\n", ping->target);
    printf("%d packets transmitted, %d received, %.0f%% packet loss, time %ldms\n",
        ping->stats.n_sent,
        ping->stats.n_received,
        (1.0 - (double)ping->stats.n_received / ping->stats.n_sent) * 100.0,
        total_ms
    );

    if (ping->stats.n_received > 0)
    {
        double avg = ping->stats.total_rtt / ping->stats.n_received;

        double var = (ping->stats.total_rtt_squared / ping->stats.n_received) - (avg * avg);
        double mdev = sqrt(var > 0 ? var : 0);

        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
            ping->stats.min_rtt,
            avg,
            ping->stats.max_rtt,
            mdev
        );
    }
    return 0;
}


// **
// ** Main ping loop functions
int main(int ac, char **av)
{
    if (getuid() != 0)
    {
        fprintf(stderr, "Error: This program must be run as root.\n");
        return (1);
    }

    t_ping ping = {
        .verbose = 0,
        .target = NULL,
        .sockfd = -1,
        .seq = 1,
        .pid = getpid() & 0xFFFF,

        .stats = {
            .n_sent = 0,
            .n_received = 0,
            .min_rtt = 1e9,
            .max_rtt = 0.0,
            .total_rtt = 0.0,
            .total_rtt_squared = 0.0
        }
    };

    int err = parse_args(&ping,  ac, av);
    if (err != 0)
        return (err);

    if (init_socket(&ping) < 0)
        return (1);

    if (resolve_host(&ping) < 0)
        return (2);

    // handle signals
    signal(SIGINT, handler);

    gettimeofday(&ping.stats.start_time, NULL);
    
    // print init message
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ping.addr.sin_addr, ip_str, sizeof(ip_str));
    if (ping.verbose)
    {
        long id = ping.pid;
        printf("PING %s (%s) 56(84) bytes of data, id %lx = %d\n",
            ping.target, ip_str, id, (int)id);
    }
    else
        printf("PING %s (%s) %d(%d) bytes of data.\n", ping.target, inet_ntoa(ping.addr.sin_addr), 56, 84);

    // loop send/recv
    while (ping_running)
    {
        if (send_echo_icmp(&ping) == 0)
            recv_echo_icmp(&ping);

        ping.seq++;

        if (!ping_running)
            break;
            
        usleep(1000000); // 1 sec
    }

    print_stats(&ping);
    return (0);
}