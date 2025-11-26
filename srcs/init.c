/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:33:44 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/26 11:44:24 by cpoulain         ###   ########.fr       */
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
    memset(ping->resolved_target, 0, sizeof(ping->resolved_target));
    memset(ping->replier_ip, 0, sizeof(ping->replier_ip));
    ping->args = (t_args){0};
	ping->id = (uint16_t)getpid() & 0xFFFF;
    ping->seq = -1;
    ping->args.interval = 1;
    ping->args.ttl = 0;
    ping->args.packet_count = 0;
    ping->args.packet_size = DEFAULT_PAYLOAD_SIZE;
    ping->data = (struct iphdr){0};
    memset(&ping->icmp_hdr_copy, 0, sizeof(struct icmphdr));
    ping->len = 0;
}