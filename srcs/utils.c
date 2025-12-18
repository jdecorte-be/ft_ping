#include "ping.h"

// **
// ** Print ping statistics
// **
int print_stats(t_ping *ping)
{
    // print the statistics
    struct timeval end_time, diff;
	gettimeofday(&end_time, NULL);
	timersub(&end_time, &ping->stats.start_time, &diff);
	long total_ms = diff.tv_sec * 1000 + diff.tv_usec / 1000;

    printf("--- %s ping statistics ---\n", ping->args.target);
    printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n",
        ping->stats.n_sent,
        ping->stats.n_received,
        (1.0 - (double)ping->stats.n_received / ping->stats.n_sent) * 100.0
    );

    if (ping->stats.n_received)
    {
        double avg = ping->stats.total_rtt / ping->stats.n_received;

        double var = (ping->stats.total_rtt_squared / ping->stats.n_received) - (avg * avg);
        double mdev = sqrt(var > 0 ? var : 0);

        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
            ping->stats.min_rtt,
            avg,
            ping->stats.max_rtt,
            mdev
        );
    }
    return 0;
}