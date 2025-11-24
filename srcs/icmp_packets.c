/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_packets.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:41:19 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/24 15:52:19 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/linker.h"
#include "../includes/ft_ping.h"

/// @brief Calcule le checksum 16 bits sur une zone memoire. Implementation classique ICMP (InternetChecksum)
/// @param buffer Pointeur vers la zone memoire a sum (void *) pour de la flexinilite. La memoire est interpretee comme une sequence d'octets
/// @param len Nombre d'octets  a prendre en compte dans le calcul
/// @return checksum 16 bits en complement a un (valeur a passer en en-tete)
uint16_t checksum(void *buffer, int len)
{
    uint32_t sum =0; // accumulateur 32 bits pour stocker les reports
    uint16_t *data = buffer; // a traiter par mots de 16 bits quand c'est possible
    
    while (len > 1)
    {
        sum += *data++; // ajoute le mot suivant et avance le pointeur
        len -= 2; // on a consomme 2 octets
    }
    // s'il reste un octet, on le traite en MSB d'un mot de 16 bits
    if (len == 1)
    {
        uint16_t last = 0;
        *(uint8_t*)&last = *(uint8_t*)data;
        sum += last;
    }

    // replier les reports de 32 bits vers 16 bits
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    // retourner le complement a un du resultat (format standard checksum internet)
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