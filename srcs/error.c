/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:24:28 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/25 12:52:32 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void   exit_ping(ArgParser *parser, byte help)
{
    int exit_code = (parser->last_error != ARGPARSE_OK && !help ? parser->last_error : 0);
    
    exit_code ? print_argparse_error(parser) : print_usage(parser);
    free_arg_parser(parser);
    exit(exit_code);
}