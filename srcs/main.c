/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:43:44 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/24 15:48:46 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ping.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr>    +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:30:00 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/24 15:30:00 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparser.h"

/* ----------------------------- Error printing ----------------------------- */

static void print_argparse_error(ArgParseResult r)
{
	if (r == ARGPARSE_OK)
		return;
	fprintf(stderr, "ft_ping: argument parsing error: ");
	if (r == ARGPARSE_ERR_TOO_MANY_OPTIONS)
		fprintf(stderr, "too many options\n");
	else if (r == ARGPARSE_ERR_TOO_MANY_POSITIONALS)
		fprintf(stderr, "too many positional arguments\n");
	else if (r == ARGPARSE_ERR_MISSING_VALUE)
		fprintf(stderr, "missing value for option or positional\n");
	else if (r == ARGPARSE_ERR_INVALID_INT)
		fprintf(stderr, "invalid integer value\n");
	else if (r == ARGPARSE_ERR_ALLOC)
		fprintf(stderr, "memory allocation failure\n");
	else
		fprintf(stderr, "unknown error\n");
}

/* ------------------------------- Main entry ------------------------------- */

int	main(int argc, char *argv[])
{
	ArgParser       parser;
	ArgParseResult  res;
    Option          opts[3] = {0};

	byte            verbose = 0;
	byte            help = 0;
	char            *host = NULL;

	init_arg_parser(&parser);

	/* ---- Declare options ---- */

	opts[0] = build_flag_option(
		"-v", "--verbose",
		&verbose,
		"Verbose output (show packet/error details)"
	);
	opts[1] = build_flag_option(
		"-?", "--help",
		&help,
		"Display this help and exit"
	);
    opts[2] = (Option){0};

    for (int i = 0; opts[i].value; i++)
    {
	    res = add_option(&parser, &opts[i]);
	    if (res != ARGPARSE_OK)
	    	return (print_argparse_error(res), 1);
    }

	res = add_positional(
		&parser, "host", &host,
        "IPv4 address or hostname", 1
	);
	if (res != ARGPARSE_OK)
		return(print_argparse_error(res), 1);

	res = parse_arguments(&parser, argc, argv);
	if (res != ARGPARSE_OK && !help)
		return (print_argparse_error(res), print_usage(&parser), free_arg_parser(&parser), 1);

	if (help)
		return (printf("Usage: ft_ping host [-v -?]\n\n"), print_usage(&parser), free_arg_parser(&parser), 0);

	if (!host)
		return (fprintf(stderr, "ft_ping: missing host operand\n"), fprintf(stderr, "Usage: ft_ping [-v -?] host\n"), free_arg_parser(&parser), 1);

	if (verbose)
		printf("ft_ping: verbose mode enabled\n");

	printf("ft_ping: target host = %s\n", host);
	free_arg_parser(&parser);
	return (0);
}
