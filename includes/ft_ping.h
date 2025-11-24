/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:36:58 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/24 15:16:13 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <stdint.h>
# include <netinet/in.h>

#define PAYLOAD_SIZE 56
#define RECV_BUF_SIZE 1500

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

#endif