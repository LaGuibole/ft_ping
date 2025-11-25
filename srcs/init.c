/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:33:44 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/25 15:25:50 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

void init_ping(t_ping *ping)
{
    ping->sockfd = -1;
    ping->parser = (ArgParser){0};
    ping->target = (struct sockaddr_in){0};
    ping->resolved_target[0] = '\0';
    ping->args = (t_args){0};
	ping->id = (uint16_t)getpid() & 0xFFFF;
    ping->seq = 0;
    ping->args.interval = 1;
    ping->args.ttl = 0;
}