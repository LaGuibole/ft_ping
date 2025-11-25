/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:22:56 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 14:12:35 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

double timeval_diff_ms(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) * 1000 + 
            (end->tv_usec - start->tv_usec) / 1000;
}

void print_reply(t_ping *ping, const struct sockaddr_in *from, int bytes, int ttl, double rtt)
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