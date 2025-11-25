/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linker.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:21:24 by guphilip          #+#    #+#             */
/*   Updated: 2025/11/25 17:47:38 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINKER_H
# define LINKER_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <errno.h>
#include <math.h>
#include <float.h>
#include <signal.h>
#include "ft_ping.h"

extern volatile sig_atomic_t g_stop;

uint16_t    checksum(void *buffer, int len);
int         build_icmp_echo(uint8_t *buffer, uint16_t id, uint16_t seq);
int         receive_packet(t_ping *ping, double *out_rtt, int *out_ttl, int *out_bytes, struct sockaddr_in *out_from);
int         resolve_target(const char* target, struct sockaddr_in *out);
int         send_packet(t_ping *ping);
int         open_icmp_socket(void);
void        stats_init(t_ping *ping);
void        stats_update(t_ping *ping, double rtt);
void        stats_print(const t_ping *ping);
double      timeval_diff_ms(struct timeval *start, struct timeval *end);
void        print_reply(t_ping *ping, int bytes, int ttl, double rtt);
void        print_ttl_exceeded(t_ping *ping, int bytes);
void        print_ttl_exceeded_dump(t_ping *ping);
int         validate_int_min_max(int value, int min, int max);

#endif
