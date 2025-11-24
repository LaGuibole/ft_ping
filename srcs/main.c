/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:05:33 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/24 15:12:04 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/linker.h"
#include "../includes/ft_ping.h"

volatile sig_atomic_t g_stop = 0;

static void sigint_handler(int sig)
{
    (void)sig;
    g_stop = 1;
}

static void print_reply(t_ping *ping, const struct sockaddr_in *from, int bytes, int ttl, double rtt)
{
    char from_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &from->sin_addr, from_ip, sizeof(from_ip));

    printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
           bytes,
           from_ip,
           ping->seq,
           ttl,
           rtt);
}

int main(int argc, char **argv)
{
    t_ping ping;
    memset(&ping, 0, sizeof(ping));
    ping.id = (uint16_t)(getpid() & 0xFFFF);
    ping.seq = 0;

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <hostname|ip>\n", argv[0]);
        return 1;
    }

    if (resolve_target(argv[1], &ping.target) < 0)
        return 1;

    inet_ntop(AF_INET, &ping.target.sin_addr, ping.target_str, sizeof(ping.target_str));

    ping.sockfd = open_icmp_socket();
    if (ping.sockfd < 0)
        return 1;

    /* set a reasonable recv timeout so we don't block forever */
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(ping.sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    stats_init(&ping);

    printf("PING %s (%s): %d data bytes\n", argv[1], ping.target_str, PAYLOAD_SIZE);

    /* install SIGINT handler */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    while (!g_stop)
    {
        if (send_packet(&ping) < 0)
        {
            fprintf(stderr, "ft_ping: send_packet failed\n");
            break;
        }

        double rtt;
        int ttl, bytes;
        struct sockaddr_in from;
        int r = receive_packet(&ping, &rtt, &ttl, &bytes, &from);

        if (r == 0)
        {
            stats_update(&ping, rtt);
            print_reply(&ping, &from, bytes, ttl, rtt);
        }
        else if (r == 1)
        {
            /* timeout for this seq */
            printf("Request timeout for icmp_seq %u\n", ping.seq);
        }
        else if (r == 2)
        {
            /* received ICMP but not echo reply (Time Exceeded, Unreach, ...), ignore or -v */
            if (0) /* replace 0 by verbose flag check */
                printf("Info: received ICMP type != ECHOREPLY\n");
        }
        else if (r == 3)
        {
            /* reply for another process, ignore */
        }
        else
        {
            /* fatal error */
            break;
        }

        /* wait ~1 second between sends (simple behavior like ping) */
        for (int i = 0; i < 10 && !g_stop; ++i)
            sleep(100000); /* 0.1s x 10 = 1s */
    }

    /* print summary */
    stats_print(&ping);

    close(ping.sockfd);
    return 0;
}