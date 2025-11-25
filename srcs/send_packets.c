/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_packets.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:48:25 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 16:29:03 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

/// @brief Construit et envoie une echo request ICMP vers la cible configuree
/// @param ping Contexte courant contenant l'identifiant, socket, l'adresse de la target
/// @return 0 si succes, sinon -1
int send_packet(t_ping *ping)
{
    uint8_t buffer[sizeof(struct icmphdr) + PAYLOAD_SIZE];
    int pkt_len;
    ssize_t sent;

    if (!ping)
        return -1;
    
    ping->seq += 1;
    pkt_len = build_icmp_echo(buffer, ping->id, ping->seq);
    if (pkt_len <= 0)
    {
        fprintf(stderr, "ft_ping: build_icmp_echo failed\n");
        return -1;
    }

    sent = sendto(ping->sockfd, buffer, pkt_len, 0, (struct sockaddr *)&ping->target, sizeof(ping->target));
    if (sent < 0)
    {
        perror("ft_ping: sendto");
        return -1;
    }
    
    ping->transmitted += 1;
    return 0;
}