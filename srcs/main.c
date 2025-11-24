/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:43:44 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/25 12:23:40 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ping.h"

/* ------------------------------- Main entry ------------------------------- */

int	main(int argc, char *argv[])
{
	ArgParser       parser;
    Option          opts[OPTION_COUNT] = {0};

	byte            verbose = 0;
	byte            help = 0;
	char            *host = NULL;

	init_arg_parser(&parser, "ft_ping");
	opts[0] = build_flag_option("-v", "--verbose", &verbose, "Verbose output (show packet/error details)");
	opts[1] = build_flag_option("-?", "--help", &help, "Display this help and exit");
    if (add_options(&parser, opts, OPTION_COUNT)
        || add_positional(&parser, "host", &host, "IPv4 address or hostname", 1)
        || parse_arguments(&parser, argc, argv))
            exit_ping(&parser, help);

	if (help)
		exit_ping(&parser, help);

	if (verbose)
		printf("ft_ping: verbose mode enabled\n");

	printf("ft_ping: target host = %s\n", host);
	free_arg_parser(&parser);
	return (0);
}
