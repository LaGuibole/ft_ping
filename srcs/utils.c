/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:22:56 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 16:29:45 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

/// @brief Fontion pour exprimer le temps en ms
/// @param start Debut timer
/// @param end Fin timer
/// @return La difference start - end exprime en ms
double timeval_diff_ms(struct timeval *start, struct timeval *end)
{
    return (double)(end->tv_sec - start->tv_sec) * 1000 + 
            (double)(end->tv_usec - start->tv_usec) / 1000;
}

void print_reply(t_ping *ping, int bytes, int ttl, double rtt)
{
    printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
           bytes,
           ping->resolved_target,
           ping->seq,
           ttl,
           rtt);
}

void print_ttl_exceeded(t_ping *ping, int bytes)
{
    printf("%d bytes from %s: Time to live exceeded\n",
           bytes,
           ping->resolved_target);
}

int validate_int_min_max(int value, int min, int max)
{
    if (value < min || value > max)
        return (printf("ft_ping: option value too big: %d\n", value), 1);
    return 0;
}