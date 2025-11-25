/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:33:44 by cpoulain          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/11/25 15:25:50 by cpoulain         ###   ########.fr       */
=======
/*   Updated: 2025/11/25 15:28:11 by guphilip         ###   ########.fr       */
>>>>>>> 8699c4d (+ | Add doxygen comments)
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
    ping->seq = 0;
    ping->args.interval = 1;
    ping->args.ttl = 0;
}