/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:22:56 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/26 11:45:34 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

static void print_hex_line(const uint8_t *buffer, ssize_t len);

/// @brief Fontion pour exprimer le temps en ms
/// @param start Debut timer
/// @param end Fin timer
/// @return La difference start - end exprime en ms
double timeval_diff_ms(struct timeval *start, struct timeval *end)
{
    long sec_diff = end->tv_sec - start->tv_sec;
    long usec_diff = end->tv_usec - start->tv_usec;
    
    // gestion du report pour les reception autre paquet
    if (usec_diff < 0) {
        sec_diff--;
        usec_diff += 1000000;
    }
    
    return (double)sec_diff * 1000.0 + (double)usec_diff / 1000.0;
}

void print_reply(t_ping *ping, int bytes, int ttl, double rtt)
{
    printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
           bytes,
           ping->resolved_target,
           ping->seq,
           ttl,
           rtt);
}

void print_ttl_exceeded(t_ping *ping, int bytes)
{
    char *ip_str = ping->replier_ip[0] == '\0' ? ping->resolved_target : ping->replier_ip;
    char hostname[NI_MAXHOST] = {0};
    
    // Try reverse DNS lookup
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip_str, &addr.sin_addr);
    
    int result = getnameinfo((struct sockaddr *)&addr, sizeof(addr),
                            hostname, sizeof(hostname),
                            NULL, 0, 0);
    
    if (result == 0 && hostname[0] != '\0' && strcmp(hostname, ip_str) != 0)
    {
        printf("%d bytes from %s (%s): Time to live exceeded\n",
               bytes, hostname, ip_str);
    }
    else
    {
        printf("%d bytes from %s: Time to live exceeded\n",
               bytes, ip_str);
    }
}

void print_ttl_exceeded_dump(t_ping *ping)
{
    if (!ping || ping->len <= 0 || memcmp(&ping->icmp_hdr_copy, &(struct icmphdr){0}, sizeof(struct icmphdr)) == 0)
        return;
    
    int ip_hlen = ping->data.ihl * 4;
    uint16_t tot_len = ntohs(ping->data.tot_len);
    uint16_t id = ntohs(ping->data.id);
    
    uint16_t frag = ntohs(ping->data.frag_off);
    int flags = (frag & 0xE000) >> 13;
    int offset = frag & 0x1FFF;

    char src[INET_ADDRSTRLEN] = {0};
    char dest[INET_ADDRSTRLEN] = {0};
    struct in_addr a;
    a.s_addr = ping->data.saddr;
    inet_ntop(AF_INET, &a, src, sizeof(src));
    a.s_addr = ping->data.daddr;
    inet_ntop(AF_INET, &a, dest, sizeof(dest));

    printf("IP Hdr Dump:\n");
    print_hex_line((uint8_t*)&ping->data, ping->len > 28 ? 28 : ping->len);
    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
    printf(" %u  %u  %02x %04x %04x   %u %04x  %02u  %02u %04x  %s  %s \n",
        ping->data.version,
        ping->data.ihl,
        ping->data.tos,  
        tot_len,        
        id,
        flags,
        offset,
        ping->data.ttl,
        ping->data.protocol,
        ntohs(ping->data.check),
        src,
        dest);

    if (ping->len >= ip_hlen + (int)sizeof(struct icmphdr))
    {
        uint16_t inner_id = ntohs(ping->icmp_hdr_copy.un.echo.id);
        uint16_t inner_seq = ntohs(ping->icmp_hdr_copy.un.echo.sequence);
        int inner_size = tot_len - ip_hlen;
        printf("ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04x\n",
               ping->icmp_hdr_copy.type,
               ping->icmp_hdr_copy.code,
               inner_size,
               inner_id,
               inner_seq);
    }
}

int validate_int_min_max(int value, int min, int max)
{
    if (value < min || value > max)
        return (printf("ft_ping: option value too big: %d\n", value), 1);
    return 0;
}

//  static declarations

static void print_hex_line(const uint8_t *buffer, ssize_t len)
{
    for (ssize_t i = 0; i < len; ++i)
    {
        printf("%02x", buffer[i]);
        if (i%2)
            printf(" ");   
    }
    if (len % 16)
        printf("\n");
}