/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:54:06 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/26 11:02:03 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

/// @brief Recoit et valide une echo reply ICMP, extrait le temps
/// @param ping Contexte ping pour acceder a l'ID ICMP et le socket
/// @param out_rtt Pointeur opt. pour stocker le RTT en ms
/// @param out_ttl Pointeur opt. pour stocker le TTL extrait depuis l'en tete ICMP
/// @param out_bytes Pointeur opt. pour stocker le nombre de bytes recus apres l'en tete IP
/// @param out_from Pointeur opt. pour stocker l'adresse du sender
/// @return 0 au succes, 1 si le paquet n'est pas encore disponible, -1 sur une erreur systeme, 2 si le 
///         paquet n'est pas une echo reply, 3 si l'ID ne matche pas
int receive_packet(t_ping *ping, double *out_rtt, int *out_ttl, int *out_bytes, struct sockaddr_in *out_from)
{
    uint8_t buffer[RECV_BUF_SIZE];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    struct timeval tv_recv;
    
    // Initialize output parameters
    if (out_rtt) *out_rtt = 0.0;
    if (out_ttl) *out_ttl = 0;
    if (out_bytes) *out_bytes = 0;
    if (out_from) memset(out_from, 0, sizeof(*out_from));
    
    ssize_t nbytes = recvfrom(ping->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &fromlen);
    
    if (nbytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return RPL_TIMEO;
        perror("recvfrom");
        return -1;
    }

    gettimeofday(&tv_recv, NULL);

    if (nbytes < (ssize_t)sizeof(struct iphdr))
        return RPL_TIMEO;

    struct iphdr *ip_hdr = (struct iphdr *)buffer;
    int ip_hlen = ip_hdr->ihl * 4;

    if (nbytes < ip_hlen + (int)sizeof(struct icmphdr))
        return RPL_TIMEO;
        
    struct icmphdr *icmp_hdr = (struct icmphdr *)(buffer + ip_hlen);
    // set les valeurs pour les paquets avec IP valide
    if (out_ttl)
        *out_ttl = ip_hdr->ttl;
    if (out_bytes)
        *out_bytes = (int)(nbytes - ip_hlen);

    if (icmp_hdr->type == ICMP_TIME_EXCEEDED)
    {
        // en tte IP originale dans payload ICMP, apres header ICMP
        uint8_t *inner_ip_ptr = (uint8_t *)icmp_hdr + sizeof(struct icmphdr);
        struct iphdr *inner_ip = (struct iphdr *)inner_ip_ptr;    
        // verif des donnees dispo
        ssize_t inner_available = nbytes - ip_hlen - sizeof(struct icmphdr);
        if (inner_available < (ssize_t)sizeof(struct iphdr))
            return RPL_NOECHO;     
        // verif validite inner_ip
        if (inner_ip->version != 4 || inner_ip->ihl < 5 || inner_ip->protocol != IPPROTO_ICMP)
            return RPL_NOECHO;

        int inner_ip_hlen = inner_ip->ihl * 4;      
        // verif donnes ICMP interne
        if (inner_available < inner_ip_hlen + (ssize_t)sizeof(struct icmphdr))
            return RPL_NOECHO;
            
        struct icmphdr *inner_icmp = (struct icmphdr *)(inner_ip_ptr + inner_ip_hlen);   
        // ECHO REQUEST + notre ping ? 
        if (inner_icmp->type != ICMP_ECHO || ntohs(inner_icmp->un.echo.id) != ping->id)
        {
            // dans le cas d'un mauvais paquet
            return RPL_NOECHO;
        }   
        // paquet actuel ?
        if (ntohs(inner_icmp->un.echo.sequence) != ping->seq)
        {
            // = ancien paquet
            return RPL_NOECHO;
        }
        // notre paquet, on copie pour le dump ttl
        ping->data = *inner_ip;
        
        static struct icmphdr icmp_copy;
        icmp_copy = *inner_icmp;
        ping->icmp_hdr = &icmp_copy;
        ping->len = (int)inner_available;
        
        // calcul RTT
        ssize_t payload_available = inner_available - inner_ip_hlen - sizeof(struct icmphdr);
        if (payload_available >= (ssize_t)sizeof(struct timeval))
        {
            uint8_t *inner_payload = inner_ip_ptr + inner_ip_hlen + sizeof(struct icmphdr);
            struct timeval *tv_sent = (struct timeval *)inner_payload;
            double rtt = timeval_diff_ms(tv_sent, &tv_recv);
            
            if (out_rtt)
                *out_rtt = rtt;
        }
        
        if (out_from)
            memcpy(out_from, &from, sizeof(from));
        return RPL_TTL_EXCEEDED;
    }
    if (out_from)
        memcpy(out_from, &from, sizeof(from));
    if (icmp_hdr->type != ICMP_ECHOREPLY)
        return RPL_NOECHO;
    if (ntohs(icmp_hdr->un.echo.id) != ping->id)
        return RPL_NOECHO;       
    // reponse au paquet courant ? 
    if (ntohs(icmp_hdr->un.echo.sequence) != ping->seq)
        return RPL_NOECHO;
    uint8_t *payload = (uint8_t *)icmp_hdr + sizeof(struct icmphdr);
    struct timeval *tv_sent = (struct timeval *)payload;

    double rtt = timeval_diff_ms(tv_sent, &tv_recv);   
    if (out_rtt)
        *out_rtt = rtt;
        
    return RPL_ECHO;
}
