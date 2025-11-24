/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:22:56 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/24 14:24:50 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"
#include "../includes/linker.h"

double timeval_diff_ms(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) * 1000 + 
            (end->tv_usec - start->tv_usec) / 1000;
}