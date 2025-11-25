/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_icmp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:20:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 15:27:04 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

/// @brief Ouvre un socket RAW ICMP et renvoie son fd
/// @param void
/// @return FD si succes, sinon -1
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