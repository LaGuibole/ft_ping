/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_addr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:57:08 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 12:52:27 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

int resolve_target(const char* target, struct sockaddr_in *out)
{
    struct addrinfo hints;
    struct addrinfo *res = NULL;
    int ret;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    
    ret = getaddrinfo(target, NULL, &hints, &res);
    if (ret != 0)
    {
        fprintf(stderr, "ft_ping: cannot resolve %s: %s\n", target, gai_strerror(ret));
        return -1;
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
    memcpy(out, addr, sizeof(struct sockaddr_in));

    freeaddrinfo(res);
    return 0;
}