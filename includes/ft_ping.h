/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:36:58 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 12:52:34 by cpoulain         ###   ########.fr       */
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

typedef struct s_ping
{
    int sockfd;
    struct sockaddr_in target;
    char target_str[64];
    uint16_t id;
    uint16_t seq;
    int transmitted;
    int received;
    double rtt_min;
    double rtt_max;
    double rtt_sum;
    double rtt_sum2;
}   t_ping;


void   exit_ping(ArgParser *parser, byte help);

#endif