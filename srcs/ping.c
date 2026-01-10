#include "ping.h"

/**
 * @brief Calculate the Internet Checksum (RFC 1071)
 *
 * @param buf Pointer to the data buffer
 * @param len Length of the data in bytes
 * @return int The 16-bit one's complement sum
 */
int checksum(uint16_t *buf, int len)
{
    uint32_t sum = 0;

    // -- sum 16-bit words --
    while (len > 1)
    {
        sum += *buf++;
        len -= 2;
    }

    // -- handle odd byte --
    if (len == 1)
    {
        sum += *(uint8_t *)buf;
    }

    // -- fold 32-bit sum to 16 bits --
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}

/**
 * @brief Construct and send an ICMP Echo Request packet
 *
 * @param ping Pointer to the ping session structure
 * @return int 0 on success, -1 on error
 */
int send_echo_icmp(t_ping *ping)
{
    int sent_bytes;

    // -- prepare packet buffer --
    unsigned char packet[sizeof(icmphdr_t)];
    memset(packet, 0, sizeof(packet));

    // -- fill ICMP header --
    icmphdr_t *icmp = (icmphdr_t *)packet;

    icmp->type = ICMP_ECHO;
    icmp->un.echo.id = htons(ping->pid);
    icmp->un.echo.sequence = htons(ping->seq);
    icmp->checksum = checksum((uint16_t *)packet, sizeof(packet));

    gettimeofday(&ping->stats.send_time, NULL);

    // -- send packet --
    sent_bytes = sendto(
        ping->sockfd,
        packet,
        sizeof(packet),
        0,
        (struct sockaddr *)&ping->addr,
        sizeof(ping->addr));

    if (sent_bytes < 0)
        error(EXIT_FAILURE, errno, "sending packet");

    ping->stats.n_sent++;

    return (0);
}

/**
 * @brief Receive and process ICMP Echo Reply packets
 *
 * @param ping Pointer to the ping session structure
 * @return int 0 on success, -1 on timeout or error
 */
int recv_echo_icmp(t_ping *ping)
{
    char packet[1024];
    char control_buf[512]; 
    
    struct iovec iov[1];
    struct msghdr msg;
    struct cmsghdr *cmsg;
    fd_set read_fds;

    while (ping_running)
    {
        FD_ZERO(&read_fds);
        FD_SET(ping->sockfd, &read_fds);

        struct timeval timeout = {1, 0};
        int ret = select(ping->sockfd + 1, &read_fds, NULL, NULL, &timeout);

        if (ret < 0)
        {
            perror("select");
            return (-1);
        }
        if (ret == 0) // Timeout
            return (-1);

        // -- Prepare recvmsg structures --
        iov[0].iov_base = packet;
        iov[0].iov_len = sizeof(packet);

        memset(&msg, 0, sizeof(msg));
        msg.msg_name = &ping->addr;
        msg.msg_namelen = sizeof(ping->addr);
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;
        msg.msg_control = control_buf;
        msg.msg_controllen = sizeof(control_buf);

        // -- Receive data --
        ssize_t recv_bytes = recvmsg(ping->sockfd, &msg, 0);
        if (recv_bytes < 0)
            return (-1);

        // -- Extract TTL from Ancillary Data (CMSG) --
        int ttl = 0;
        for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg))
        {
            if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_TTL)
            {
                ttl = *(int *)CMSG_DATA(cmsg);
                break;
            }
        }

        // -- Process Pointers based on Socket Type --
        struct ip *ip = NULL;
        icmphdr_t *icmp;
        size_t ip_header_len = 0;

        if (ping->socktype == SOCK_DGRAM)
        {
            if ((size_t)recv_bytes < sizeof(icmphdr_t))
                return -1;

            icmp = (icmphdr_t *)packet;
        }
        else // SOCK_RAW
        {
            ip = (struct ip *)packet;
            ip_header_len = ip->ip_hl << 2;

            if ((size_t)recv_bytes < ip_header_len + sizeof(icmphdr_t))
            {
                fprintf(stderr, "packet too short (%zd bytes) from %s\n", 
                        recv_bytes, inet_ntoa(ping->addr.sin_addr));
                return -1;
            }

            icmp = (icmphdr_t *)(packet + ip_header_len);
            
            if (ttl == 0) 
                ttl = ip->ip_ttl;
        }

        uint16_t id = ntohs(icmp->un.echo.id);
        uint16_t seq = ntohs(icmp->un.echo.sequence);
        int type = icmp->type;

        // -- Calculate RTT --
        struct timeval recv_time;
        gettimeofday(&recv_time, NULL);
        double rtt = (recv_time.tv_sec - ping->stats.send_time.tv_sec) * 1000.0 +
                     (recv_time.tv_usec - ping->stats.send_time.tv_usec) / 1000.0;

        // -- Check Reply --
        if (type == ICMP_ECHOREPLY || type == ICMP_TIMESTAMPREPLY || type == ICMP_ADDRESSREPLY)
        {
            if (ping->socktype == SOCK_RAW)
            {
                if (id != ping->pid)
                    return -1;

                unsigned short cksum = icmp->checksum;
                icmp->checksum = 0;
                if (cksum != checksum((uint16_t *)icmp, sizeof(icmphdr_t)))
                {
                    icmp->checksum = cksum; // restore
                    fprintf(stderr, "checksum mismatch from %s\n",
                            inet_ntoa(ping->addr.sin_addr));
                    return -1;
                }
                icmp->checksum = cksum; // restore
            }

            // -- Update Stats --
            ping->stats.n_received++;
            ping->stats.min_rtt = fmin(ping->stats.min_rtt, rtt);
            ping->stats.max_rtt = fmax(ping->stats.max_rtt, rtt);
            ping->stats.total_rtt += rtt;
            ping->stats.total_rtt_squared += rtt * rtt;

            printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                   64,
                   inet_ntoa(ping->addr.sin_addr),
                   seq,
                   ttl,
                   rtt);

            return (0);
        }
    }
    return (0);
}