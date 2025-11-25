/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:43:44 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/25 14:24:33 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "linker.h"

volatile sig_atomic_t g_stop = 0;

/* ---------------------------- Static prototypes --------------------------- */

static int executing(t_ping *ping);
static int executing_loop(t_ping *ping);

/* ------------------------------- Main entry ------------------------------- */

int	main(int argc, char *argv[])
{
    Option          opts[OPTION_COUNT] = {0};
	t_ping			ping = {0};
	ArgParser	  	*parser = &ping.parser;
	t_args			*args = &ping.args;

	init_ping(&ping);
	init_arg_parser(parser, "ft_ping");
	opts[0] = build_flag_option("-v", "--verbose", &args->verbose, "Verbose output (show packet/error details)");
	opts[1] = build_flag_option("-?", "--help", &args->help, "Display this help and exit");
    if (add_options(parser, opts, OPTION_COUNT)
        || add_positional(parser, "host", &args->target_str, "IPv4 address or hostname", 1)
        || parse_arguments(parser, argc, argv))
            exit_ping(&ping, args->help);

	if (args->help)
		exit_ping(&ping, args->help);
	
	if (executing(&ping))
		return (fprintf(stderr, "ft_ping: execution failed (PLACEHOLDER FOR ERROR)\n"), 1);
	
	free_arg_parser(parser);
	return (0);
}

/* ------------------------- Static implementations ------------------------- */

static int executing(t_ping *ping)
{
	struct sigaction sa = {0};
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);

	if (resolve_target(ping->args.target_str, &ping->target) < 0)
		return (1);

	inet_ntop(AF_INET, &ping->target.sin_addr, ping->resolved_target, sizeof(ping->resolved_target));
	ping->sockfd = open_icmp_socket();
	if (ping->sockfd < 0)
		return (1);

	setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, &(struct timeval){2, 0}, sizeof(struct timeval));
	stats_init(ping);
	printf("PING %s (%s): %d data bytes\n", ping->args.target_str, ping->resolved_target, PAYLOAD_SIZE);
	return (executing_loop(ping));
}

static int executing_loop(t_ping *ping)
{
	while (!g_stop)
    {
        if (send_packet(ping) < 0)
        {
            fprintf(stderr, "ft_ping: send_packet failed\n");
            break;
        }

        double rtt;
        int ttl, _bytes;
        struct sockaddr_in from;
        int r = receive_packet(ping, &rtt, &ttl, &_bytes, &from);

        if (r == RPL_ECHO)
        {
            stats_update(ping, rtt);
            print_reply(ping, &from, _bytes, ttl, rtt);
        }
        else if (r == RPL_TIMEO)
		{
            printf("Request timeout for icmp_seq %u\n", ping->seq);
		}
        else if (r == RPL_NOECHO)
        {
			if (ping->args.verbose)
				printf("Info: received ICMP type != ECHOREPLY\n");
		}    
        else
            break;

		//TODO: Set flag for interval
        for (int i = 0; i < 10 && !g_stop; ++i)
            usleep(100000);
    }
    stats_print(ping);

    close(ping->sockfd);
    return 0;
}