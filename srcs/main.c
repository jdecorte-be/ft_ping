#include "ping.h"

_Bool ping_running = 1;

/**
 * @brief Signal handler to stop pinging on SIGINT
 *
 * @param signum The signal number received
 */
void handler(int signum)
{
    if (signum == SIGINT)
        ping_running = 0;
}

/**
 * @brief Print ping statistics including packet loss and RTT metrics
 *
 * @param ping Pointer to the ping session structure
 * @return int Always returns 0
 */
int print_stats(t_ping *ping)
{
    struct timeval end_time, diff;

    // -- calculate elapsed time --
    gettimeofday(&end_time, NULL);
    timersub(&end_time, &ping->stats.start_time, &diff);
    
    printf("--- %s ping statistics ---\n", ping->args.target);

    // -- print packet transmission summary --
    printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n",
           ping->stats.n_sent,
           ping->stats.n_received,
           (1.0 - (double)ping->stats.n_received / ping->stats.n_sent) * 100.0);

    // -- calculate and print RTT metrics --
    if (ping->stats.n_received)
    {
        double avg = ping->stats.total_rtt / ping->stats.n_received;
        double var = (ping->stats.total_rtt_squared / ping->stats.n_received) - (avg * avg);
        double mdev = sqrt(var > 0 ? var : 0);

        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               ping->stats.min_rtt,
               avg,
               ping->stats.max_rtt,
               mdev);
    }
    return 0;
}

const char args_doc[] = "HOST ...";
const char doc[] = "Send ICMP ECHO_REQUEST packets to network hosts."
                   "\vOptions marked with (root only) are available only to "
                   "superuser.";

static struct argp_option options[] = {
    {"verbose", 'v', NULL, 0, "verbose output"},
    {"help", '?', 0, 0, "give this help list"},
    {0}};

/**
 * @brief Parse command line options using argp
 *
 * @param key The option key
 * @param arg The option argument
 * @param state The argp state
 * @return error_t Error code or 0 on success
 */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
    case 'v':
        arguments->verbose = 1;
        break;
    case '?':
        argp_state_help(state, stdout, ARGP_HELP_STD_HELP);
        return 0;
    case ARGP_KEY_NO_ARGS:
        argp_error(state, "missing host operand");
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/**
 * @brief Resolve hostname to IP address via DNS
 *
 * @param ping Pointer to the ping session structure
 * @return int 0 on success, exits on failure
 */
int resolve_host(t_ping *ping)
{
    struct addrinfo hints = {0};
    struct addrinfo *res;
    int err;

    // -- configure address hints --
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    // -- perform resolution --
    err = getaddrinfo(ping->args.target, NULL, &hints, &res);
    if (err != 0)
        error(EXIT_FAILURE, 0, "unknown host");

    memcpy(&ping->addr, res->ai_addr, sizeof(struct sockaddr_in));
    freeaddrinfo(res);

    return (0);
}

static struct argp argp = {options, parse_opt, args_doc, doc};

/**
 * @brief Initialize raw socket or fallback to datagram socket for non-root users
 *
 * @param ping Pointer to the ping session structure
 * @return int 0 on success, -1 on failure
 */
int init_socket(t_ping *ping)
{
    // -- attempt to open raw socket --
    ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping->sockfd < 0)
    {
        if (errno == EPERM || errno == EACCES)
        {
            errno = 0;

            // -- fallback to datagram socket for unprivileged users --
            ping->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
            if (ping->sockfd < 0)
            {
                if (errno == EPERM || errno == EACCES || errno == EPROTONOSUPPORT)
                    fprintf(stderr, "ping: Lacking privilege for icmp socket.\n");
                else
                    fprintf(stderr, "ping: %s\n", strerror(errno));

                return (-1);
            }
            ping->socktype = SOCK_DGRAM;
        }
    }
    else
        ping->socktype = SOCK_RAW;
    return (0);
}

/**
 * @brief Main entry point for the ping program
 *
 * @param ac Argument count
 * @param av Argument vector
 * @return int 0 on success, non-zero on error
 */
int main(int ac, char **av)
{
    // -- initialize session structure --
    t_ping ping = {
        .args = {.target = NULL, .verbose = 0},
        .sockfd = -1,
        .seq = 0,
        .pid = getpid() & 0xFFFF,
        .stats = {.n_sent = 0, .n_received = 0, .min_rtt = 1e9, .max_rtt = 0.0, .total_rtt = 0.0, .total_rtt_squared = 0.0}
    };

    // -- parse command line --
    int index;
    argp_parse(&argp, ac, av, 0, &index, &ping.args);
    ac -= index;
    av += index;

    // -- process each target --
    while (ac--)
    {
        // -- reset target statistics --
        ping.stats = (struct s_stats){
            .n_sent = 0,
            .n_received = 0,
            .min_rtt = 1e9,
            .max_rtt = 0.0,
            .total_rtt = 0.0,
            .total_rtt_squared = 0.0};

        ping.args.target = *av++;

        if (resolve_host(&ping) < 0)
            return (1);

        if (init_socket(&ping) < 0)
            return (2);

        // -- drop root privileges --
        if (setuid(getuid()) != 0)
            error(EXIT_FAILURE, errno, "setuid");

        // -- configure socket options --
        int enable = 1;
        if (setsockopt(ping.sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable)) < 0)
            perror("setsockopt SO_BROADCAST");

        if (setsockopt(ping.sockfd, IPPROTO_IP, IP_RECVTTL, &enable, sizeof(enable)) < 0)
            perror("setsockopt IP_RECVTTL");

        // -- setup signal handling --
        signal(SIGINT, handler);

        gettimeofday(&ping.stats.start_time, NULL);

        // -- display session header --
        printf("PING %s (%s): 56 data bytes",
               ping.args.target, inet_ntoa(ping.addr.sin_addr));
        if (ping.args.verbose)
            printf(", id 0x%04x = %u", ping.pid, ping.pid);
        printf("\n");

        // -- main transmission loop --
        while (ping_running)
        {
            if (send_echo_icmp(&ping) == 0)
                recv_echo_icmp(&ping);

            ping.seq++;

            if (!ping_running)
                break;

            usleep(1000000); // 1 sec
        }

        // -- cleanup and final report --
        close(ping.sockfd);
        print_stats(&ping);
    }

    return (0);
}