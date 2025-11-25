/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_icmp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:20:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 12:52:38 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

int open_icmp_socket(void)
{
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd < 0)
    {
        perror("ft_ping: socket creation failed badly...\n");
        return -1;
    }
    // gestion flags style -v, -ttl etc... ici
    return fd;
}