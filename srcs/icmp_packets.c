/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_packets.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:41:19 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/24 14:38:44 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/linker.h"
#include "../includes/ft_ping.h"

// TODO checksum, build_icmp_echo
uint16_t checksum(void *buffer, int len)
{
    uint32_t sum =0;
    uint16_t *data = buffer;
    
    while (len > 1)
    {
        sum += *data++;
        len -= 2;
    }
    if (len == 1)
    {
        uint16_t last = 0;
        *(uint8_t*)&last = *(uint8_t*)data;
        sum += last;
    }

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    
    return ~sum;
}

int build_icmp_echo(uint8_t *buffer, uint16_t id, uint16_t seq)
{
    struct icmphdr *icmp_hdr = (struct icmphdr *)buffer;
    
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->checksum = 0;
    icmp_hdr->un.echo.id = htons(id);
    icmp_hdr->un.echo.sequence = htons(seq);
    
    // payload, on fout le timestamp au debut du payload pour les calculs de RTT
    struct timeval now;
    gettimeofday(&now, NULL);
    uint8_t *payload = buffer + sizeof(struct icmphdr);
    memcpy(payload, &now, sizeof(now));

    int payload_len = PAYLOAD_SIZE;
    for (int i = sizeof(now); i < payload_len; ++i)
        payload[i] = (uint8_t)(i & 0xFF);
    
    int total_len = sizeof(struct icmphdr) + payload_len;
    icmp_hdr->checksum = checksum((void *)icmp_hdr, total_len);

    return total_len;
}