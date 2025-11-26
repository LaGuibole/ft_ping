/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:22:10 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 16:39:54 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

/// @brief Fonction d'init des parametres de stats dans la structure ping
/// @param ping Pointeur vers la structure ping
void stats_init(t_ping *ping)
{
    if (!ping)
        return ;
    ping->transmitted = 0;
    ping->received = 0;
    ping->rtt_min = DBL_MAX;
    ping->rtt_max = 0.0;
    ping->rtt_sum = 0.0;
    ping->rtt_sum2 = 0.0;
}

/// @brief Met a jour les stats de reception et de delai apres une reponse ICMP
/// @param ping Contexte courant ping contenant les mesures
/// @param rtt RTT mesure en ms
void stats_update(t_ping *ping, double rtt)
{
    if (!ping)
        return ;
    ping->received += 1;
    
    if (rtt < ping->rtt_min)
        ping->rtt_min = rtt;
    if (rtt > ping->rtt_max)
        ping->rtt_max = rtt;
    
    ping->rtt_sum += rtt;
    ping->rtt_sum2 += rtt * rtt;
}

/// @brief Affiche les stats gloables de ping a la fin de l'execution
/// @param ping Contexte courant contenant les mesures
void stats_print(const t_ping *ping)
{
    if (!ping)
        return ;
    int tx = ping->transmitted;
    int rx = ping->received;

    double loss = 0.0;
    if (tx > 0)
        loss = ((double)(tx - rx) / (double)tx) * 100;
    
    printf("\n--- %s ping statistics ---\n", ping->args.target_str);
    printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n", tx, rx, loss);

    if (rx > 0)
    {
        double avg = ping->rtt_sum / rx;

        double mean_sq = (ping->rtt_sum2 / rx) - (avg * avg);
        if (mean_sq < 0.0 && mean_sq > -1e-9)
            mean_sq = 0.0;
        
        double mdev = sqrt(mean_sq);
        
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", ping->rtt_min, avg, ping->rtt_max, mdev);
    }
}