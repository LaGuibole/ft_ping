/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:33:44 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/25 18:35:00 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

/// @brief Fonction d'init de la structure ping
/// @param ping La structure Pin
void init_ping(t_ping *ping)
{
    if (!ping)
        return ;
    ping->sockfd = -1;
    ping->parser = (ArgParser){0};
    ping->target = (struct sockaddr_in){0};
    ping->resolved_target[0] = '\0';
    ping->args = (t_args){0};
	ping->id = (uint16_t)getpid() & 0xFFFF;
    ping->seq = -1;
    ping->args.interval = 1;
    ping->args.ttl = 0;
    ping->data = (struct iphdr){0};
    ping->icmp_hdr = NULL;
    ping->icmp_hdr_copy = NULL;
    ping->len = 0;
}