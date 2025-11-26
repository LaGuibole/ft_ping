/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:43:44 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/26 10:57:30 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

volatile sig_atomic_t g_stop = 0;

/* ---------------------------- Static prototypes --------------------------- */

static int executing(t_ping *ping);
static int executing_loop(t_ping *ping);

/* ------------------------------- Main entry ------------------------------- */

int main(int argc, char *argv[])
{
	Option opts[OPTION_COUNT] = {0};
	t_ping ping = {0};
	ArgParser *parser = &ping.parser;
	t_args *args = &ping.args;

	init_ping(&ping);
	init_arg_parser(parser, "ft_ping");
	opts[0] = build_flag_option("-v", "--verbose", &args->verbose, VERBOSE_DESC);
	opts[1] = build_flag_option("-?", "--help", &args->help, HELP_DESC);
	opts[2] = build_flag_option("-q", "--quiet", &args->quiet, QUIET_DESC);
	opts[3] = build_double_option("-i", "--interval", &args->interval, INTERVAL_DESC);
	opts[4] = build_int_option(NULL, "--ttl", &args->ttl, TTL_DESC);
	opts[5] = build_int_option("-c", "--count", &args->packet_count, COUNT_DESC);
	opts[6] = build_int_option("-s", "--size", &args->packet_size, SIZE_DESC);
	if (add_options(parser, opts, OPTION_COUNT) || add_positional(parser, "host", &args->target_str, "IPv4 address or hostname", 1) || parse_arguments(parser, argc, argv))
		exit_ping(&ping, args->help);

	if (args->help)
		exit_ping(&ping, args->help);

	if (validate_int_min_max(args->ttl, 0, 255), validate_int_min_max(args->packet_size, 0, 65400))
		return (free_arg_parser(parser), 1);

	if (executing(&ping))
		return (free_arg_parser(parser), 1);

	free_arg_parser(parser);
	return (0);
}

/* ------------------------- Static implementations ------------------------- */

static int executing(t_ping *ping)
{
	struct sigaction sa = {0};
	char buffer[100];
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);

	if (resolve_target(ping->args.target_str, &ping->target) < 0)
		return (1);

	inet_ntop(AF_INET, &ping->target.sin_addr, ping->resolved_target, sizeof(ping->resolved_target));
	ping->sockfd = open_icmp_socket();
	if (ping->sockfd < 0)
		return (1);

	setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, &(struct timeval){2, 0}, sizeof(struct timeval));
	if (ping->args.ttl > 0)
		if (setsockopt(ping->sockfd, IPPROTO_IP, IP_TTL, &ping->args.ttl, sizeof(ping->args.ttl)) < 0 && (!ping->args.quiet))
			perror("ft_ping: warning - failed to set TTL");
	stats_init(ping);
	snprintf(buffer, sizeof(buffer), ", id 0x%04x = %d", ping->id, ping->id);
	printf("PING %s (%s): %d data bytes%s\n",
		   ping->args.target_str,
		   ping->resolved_target,
		   ping->args.packet_size,
		   ping->args.verbose ? buffer : "");

	return (executing_loop(ping));
}

static int executing_loop(t_ping *ping)
{
	while (!g_stop && (ping->args.packet_count == 0 || ping->transmitted < ping->args.packet_count))
	{
		if (send_packet(ping) < 0)
		{
			fprintf(stderr, "ft_ping: send_packet failed\n");
			break;
		}

		// receive loop avec timeout de 1s
		double rtt = 0.0;
		int ttl = 0, _bytes = 0;
		struct sockaddr_in from;
		int r = RPL_TIMEO;
		
		struct timeval timeout_start;
		gettimeofday(&timeout_start, NULL);
		
		while (!g_stop)
		{
			r = receive_packet(ping, &rtt, &ttl, &_bytes, &from);
			
			if (r == RPL_ECHO || r == RPL_TTL_EXCEEDED)
				break; // reponse recue pour notre paquet
				
			if (r != RPL_TIMEO && r != RPL_NOECHO)
				break; // error
				
			// verif timeout
			struct timeval now;
			gettimeofday(&now, NULL);
			double elapsed = timeval_diff_ms(&timeout_start, &now);
			if (elapsed > 1000.0) // 1s
			{
				r = RPL_TIMEO;
				break;
			}
			
			usleep(1000); // sleep pour eviter le flood
		}

		if (r == RPL_ECHO)
		{
			stats_update(ping, rtt);
			if (!ping->args.quiet)
				print_reply(ping, _bytes, ttl, rtt);
		}
		else if (r == RPL_TTL_EXCEEDED)
		{
			// Ttl execeeded pour notre paquet - afiichage du RTT
			if (rtt > 0.0)
				printf("From %s: Time to live exceeded (time=%.3f ms)\n", 
				       ping->resolved_target, rtt);
			else
				print_ttl_exceeded(ping, _bytes);
				
			if (ping->args.verbose && ping->len > 0)
				print_ttl_exceeded_dump(ping);
		}
		else if (r == RPL_TIMEO)
		{
			// timeout - pas de reponse
			if (!ping->args.quiet)
				printf("Request timeout for icmp_seq %u\n", ping->seq);
		}
		else if (r == RPL_NOECHO)
		{
			// useless maintenant ? 
		}
		else
			break;

		if (ping->transmitted == ping->args.packet_count)
			break;
		usleep(ping->args.interval * 1e6);
	}
	stats_print(ping);

	close(ping->sockfd);
	return 0;
}