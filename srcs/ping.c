#include "ping.h"

int checksum(uint16_t *buf, int len)
{
    uint32_t sum = 0;
    while (len > 1)
    {
        sum += *buf++;
        len -= 2;
    }
    if (len == 1)
    {
        sum += *(uint8_t *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}

int send_echo_icmp(t_ping *ping)
{
    int sent_bytes;
    static uint16_t seq = 0;

    // ** fill the packet **

    // header + body size
    uint8_t packet[sizeof(struct icmphdr) + ICMP_BODY_SIZE];
    memset(packet, 0, sizeof(packet));

    // fill timestamp
    struct timeval *ts = (struct timeval *)(packet + sizeof(struct icmphdr));
    if (gettimeofday(ts, NULL) < 0)
    {
        perror("gettimeofday");
        return (-1);
    }

    // fill ICMP header 
    struct icmphdr *icmp_hdr = (struct icmphdr *)packet;

    ping->pid = getpid() & 0xFFFF;

    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->un.echo.id = htons(ping->pid);
    icmp_hdr->un.echo.sequence = htons(ping->seq);
    icmp_hdr->checksum = checksum((uint16_t *)packet, sizeof(packet));

    // save send time
    gettimeofday(&ping->stats.send_time, NULL);

    // ** send packet **
    sent_bytes = sendto(
        /* socket */    ping->sockfd,
        /* buffer */    packet,
        /* length */    64,
        /* flags */     0,
        /* dest addr */ (struct sockaddr *)&ping->addr,
        /* addr len */  sizeof(ping->addr)
    );

    if (sent_bytes < 0)
    {
        perror("sendto");
        return (-1);
    }
    
    ping->stats.n_sent++;
    return (0);
}

int recv_echo_icmp(t_ping *ping)
{
    char recv_buf[1024];
    socklen_t addr_len = sizeof(ping->addr);


    // loop in case of wrong packets
    while (ping_running)
    {
        // receive packet
        int recv_bytes = recvfrom(
            /* socket */    ping->sockfd,
            /* buffer */    recv_buf,
            /* length */    sizeof(recv_buf),
            /* flags */     0,
            /* src addr */  (struct sockaddr *)&ping->addr,
            /* addr len */  &addr_len
        );
        if (recv_bytes < 0)
        {
            if (errno == EINTR) {
                return (0);
            }
            perror("recvfrom");
            return (-1);
        }

        // ** process reply ---
        struct ip *ip_hdr = (struct ip *)recv_buf;
        struct icmphdr *icmp_hdr = (struct icmphdr *)(recv_buf + (ip_hdr->ip_hl << 2));

        int type = icmp_hdr->type;
        int code = icmp_hdr->code;
        int ttl = ip_hdr->ip_ttl;
        uint16_t id, seq;

        // handle time exceeded and dest unreachable
        if (type == ICMP_TIME_EXCEEDED || type == ICMP_DEST_UNREACH)
        {
            // extract encapsulated ICMP header
            struct ip *orig_ip_hdr = (struct ip *)(recv_buf + (ip_hdr->ip_hl << 2) + sizeof(struct icmphdr));
            struct icmp *orig_icmp = (struct icmp *)((uint8_t *)orig_ip_hdr + (orig_ip_hdr->ip_hl << 2));

            id = ntohs(orig_icmp->icmp_id);
            seq = ntohs(orig_icmp->icmp_seq);
        }
        else
        {
            id = ntohs(icmp_hdr->un.echo.id);
            seq = ntohs(icmp_hdr->un.echo.sequence);
        }

        // ignore, the packet is not for us
        if (id != ping->pid)
            continue;

        // ignore, probably old packet ou duplicate
        if (seq != ping->seq)
            continue;

        // calcul RTT
        struct timeval recv_time;
        gettimeofday(&recv_time, NULL);
        double rtt = (recv_time.tv_sec - ping->stats.send_time.tv_sec) * 1000.0 + 
            (recv_time.tv_usec - ping->stats.send_time.tv_usec) / 1000.0;

        if (type == ICMP_ECHOREPLY)        
        {
            ping->stats.n_received++;

            ping->stats.min_rtt = fmin(ping->stats.min_rtt, rtt);
            ping->stats.max_rtt = fmax(ping->stats.max_rtt, rtt);
            ping->stats.total_rtt += rtt;
            ping->stats.total_rtt_squared += rtt * rtt;

            printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                recv_bytes - (ip_hdr->ip_hl << 2),
                inet_ntoa(ping->addr.sin_addr),
                ntohs(icmp_hdr->un.echo.sequence),
                ip_hdr->ip_ttl,
                rtt
            );
            return 0;
        }
        else if (type == ICMP_TIME_EXCEEDED)
        {
            printf("From %s icmp_seq=%d Time to live exceeded\n",
                inet_ntoa(ping->addr.sin_addr),
                seq
            );
            return -1;
        }
        else if (type == ICMP_DEST_UNREACH)
        {
            printf("From %s icmp_seq=%d Destination unreachable (code %d)\n",
                inet_ntoa(ping->addr.sin_addr),
                seq,
                code
            );
            return -1;
        }
    }


    return (0);
}
