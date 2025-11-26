/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:36:58 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/26 10:18:57 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <stdint.h>
# include <netinet/in.h>
# include "argparser.h"

#define DEFAULT_PAYLOAD_SIZE 56
#define RECV_BUF_SIZE 1500
#define OPTION_COUNT 7

#define RPL_ECHO 0
#define RPL_TIMEO 1
#define RPL_NOECHO 2
#define RPL_TTL_EXCEEDED 3

#define VERBOSE_DESC "verbose output"
#define QUIET_DESC "quiet output"
#define HELP_DESC "give this help list"
#define TTL_DESC "specify N as time-to-live"
#define SIZE_DESC "Specifies the number of data bytes to be sent. The default is 56, which translates into 64 ICMP data bytes, taking the 8 bytes of ICMP header data into account."
#define COUNT_DESC "Stop after sending and receiving answers to a total of n packets."
#define INTERVAL_DESC "Wait n seconds until sending next packet. The default is to wait for one second between packets. This option is incompatible with the option -f."


typedef struct s_args
{
    char        *target_str;
    byte        verbose;
    byte        help;
    byte        quiet;
    double      interval;
    int         ttl;
    int         packet_count;
    int         packet_size;
}   t_args;

typedef struct s_ping
{
    int         sockfd;
    struct      sockaddr_in target;
    char        resolved_target[INET6_ADDRSTRLEN];
    t_args      args;
    ArgParser   parser;
    uint16_t    id;
    uint16_t    seq;
    int         transmitted;
    int         received;
    double      rtt_min;
    double      rtt_max;
    double      rtt_sum;
    double      rtt_sum2;
    struct iphdr data;
    struct icmphdr *icmp_hdr;
    struct icmphdr *icmp_hdr_copy;
    ssize_t     len;
}   t_ping;

void    init_ping(t_ping *ping);
void    exit_ping(t_ping *ping, byte help);
void    sigint_handler(int sig);

#endif