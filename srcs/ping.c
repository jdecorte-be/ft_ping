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
        /* length */    sizeof(packet),
        /* flags */     0,
        /* dest addr */ (struct sockaddr *)&ping->addr,
        /* addr len */  sizeof(ping->addr)
    );

    if (sent_bytes < 0)
        error(EXIT_FAILURE, errno, "sending packet");

    ping->stats.n_sent++;
    
    return (0);
}

int recv_echo_icmp(t_ping *ping)
{
    char recv_buf[1024];
    socklen_t addr_len = sizeof(ping->addr);
    fd_set read_fds;


    // loop in case of wrong packets
    while (ping_running)
    {

        FD_ZERO(&read_fds);
        FD_SET(ping->sockfd, &read_fds);

        struct timeval timeout = {1, 0}; // 1 sec
        int ret = select(ping->sockfd + 1, &read_fds, NULL, NULL, &timeout);
        if (ret < 0)
        {
            perror("select");
            return (-1);
        }
        if (ret == 0) // timeout
            return (-1);

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
            return (-1);

        // ** process reply **

        size_t hlen;
        unsigned short cksum;
        struct ip *ip;
        icmphdr_t *icmp;

        ip = (struct ip *)recv_buf;
        hlen = ip->ip_hl << 2;
        icmp = (icmphdr_t *)(recv_buf + hlen);



        int type = icmp->type;
        int code = icmp->code;
        int ttl = ip->ip_ttl;
        
        // Extract ID and sequence for all packet types
        uint16_t id = ntohs(icmp->un.echo.id);
        uint16_t seq = ntohs(icmp->un.echo.sequence);

        // calcul RTT
        struct timeval recv_time;
        gettimeofday(&recv_time, NULL);
        double rtt = (recv_time.tv_sec - ping->stats.send_time.tv_sec) * 1000.0 + 
            (recv_time.tv_usec - ping->stats.send_time.tv_usec) / 1000.0;


        if (type == ICMP_ECHOREPLY || type == ICMP_TIMESTAMPREPLY || type == ICMP_ADDRESSREPLY)
        {
            if (ping->socktype == SOCK_RAW)
            {
                // verify it's for us
                if (id != ping->pid)
                    return -1;
                
                // verify checksum
                cksum = icmp->checksum;
                icmp->checksum = 0;
                if (cksum != checksum((uint16_t *)icmp, recv_bytes - hlen))
                {
                    icmp->checksum = cksum; // restore
                    fprintf(stderr, "checksum mismatch from %s\n", 
                            inet_ntoa(ping->addr.sin_addr));
                    return -1;
                }
                icmp->checksum = cksum; // restore
            }

            ping->stats.n_received++;
            ping->stats.min_rtt = fmin(ping->stats.min_rtt, rtt);
            ping->stats.max_rtt = fmax(ping->stats.max_rtt, rtt);
            ping->stats.total_rtt += rtt;
            ping->stats.total_rtt_squared += rtt * rtt;
            
            printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                recv_bytes - (ip->ip_hl << 2),
                inet_ntoa(ping->addr.sin_addr),
                ntohs(icmp->un.echo.sequence),
                ip->ip_ttl,
                rtt);
            
            return (0);
        }
    }
    return (0);
}