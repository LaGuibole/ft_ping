/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:54:06 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 14:26:41 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"
#include "ft_ping.h"

int receive_packet(t_ping *ping, double *out_rtt, int *out_ttl, int *out_bytes, struct sockaddr_in *out_from)
{
    uint8_t buffer[RECV_BUF_SIZE];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    struct timeval tv_recv;
    
    ssize_t nbytes = recvfrom(ping->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &fromlen);
    
    if (nbytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return 1;
        perror("recvfrom");
        return -1;
    }

    gettimeofday(&tv_recv, NULL);

    if (nbytes < (ssize_t)sizeof(struct iphdr))
        return -1;

    struct iphdr *ip_hdr = (struct iphdr *)buffer;
    int ip_hlen = ip_hdr->ihl * 4;

    if (nbytes < ip_hlen + (int)sizeof(struct icmphdr))
        return -1;
        
    struct icmphdr *icmp_hdr = (struct icmphdr *)(buffer + ip_hlen);

    if (icmp_hdr->type != ICMP_ECHOREPLY)
        return 2;

    if (ntohs(icmp_hdr->un.echo.id) != ping->id)
        return 3;

    uint8_t *payload = (uint8_t *)icmp_hdr + sizeof(struct icmphdr);
    struct timeval *tv_sent = (struct timeval *)payload;

    double rtt = timeval_diff_ms(tv_sent, &tv_recv);
    
    if (out_rtt)
        *out_rtt = rtt;
    if (out_ttl)
        *out_ttl = ip_hdr->ttl;
    if (out_bytes)
        *out_bytes = (int)(nbytes - ip_hlen);
    if (out_from)
        memcpy(out_from, &from, sizeof(from));
    return 0;
}
