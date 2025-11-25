/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:22:56 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 12:52:40 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include "linker.h"

double timeval_diff_ms(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) * 1000 + 
            (end->tv_usec - start->tv_usec) / 1000;
}