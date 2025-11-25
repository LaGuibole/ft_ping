/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:36:58 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 14:11:42 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <stdint.h>
# include <netinet/in.h>
# include "argparser.h"

#define PAYLOAD_SIZE 56
#define RECV_BUF_SIZE 1500
#define OPTION_COUNT 2

#define RPL_ECHO 0
#define RPL_TIMEO 1
#define RPL_NOECHO 2

typedef struct s_args
{
    char        *target_str;
    byte        verbose;
    byte        help;
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
}   t_ping;

void    init_ping(t_ping *ping);
void    exit_ping(t_ping *ping, byte help);
void    sigint_handler(int sig);

#endif