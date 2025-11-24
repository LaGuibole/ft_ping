/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:43:44 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/24 16:54:45 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ping.h"

/* ----------------------------- Error printing ----------------------------- */

static void print_argparse_error(ArgParseResult r)
{
	if (r == ARGPARSE_OK)
		return;
	fprintf(stderr, "ft_ping: argument parsing error: ");
	if (r == ARGPARSE_ERR_TOO_MANY_OPTIONS)
		fprintf(stderr, "too many options\n\n");
	else if (r == ARGPARSE_ERR_TOO_MANY_POSITIONALS)
		fprintf(stderr, "too many positional arguments\n\n");
	else if (r == ARGPARSE_ERR_MISSING_VALUE)
		fprintf(stderr, "missing value for option or positional\n\n");
	else if (r == ARGPARSE_ERR_INVALID_INT)
		fprintf(stderr, "invalid integer value\n\n");
	else if (r == ARGPARSE_ERR_ALLOC)
		fprintf(stderr, "memory allocation failure\n\n");
	else
		fprintf(stderr, "unknown error\n\n");
}

/* ------------------------------- Main entry ------------------------------- */

int	main(int argc, char *argv[])
{
	ArgParser       parser;
	ArgParseResult  res;
    Option          opts[OPTION_COUNT] = {0};

	byte            verbose = 0;
	byte            help = 0;
	char            *host = NULL;

	init_arg_parser(&parser, "ft_ping");

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

    for (int i = 0; i < OPTION_COUNT; i++)
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
	if (res != ARGPARSE_OK || help)
        return (res != ARGPARSE_OK && !help ? print_argparse_error(res) : NULL,
                print_usage(&parser),
                free_arg_parser(&parser),
                res != ARGPARSE_OK && !help);

	if (verbose)
		printf("ft_ping: verbose mode enabled\n");

	printf("ft_ping: target host = %s\n", host);
	free_arg_parser(&parser);
	return (0);
}
