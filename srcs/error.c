/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoulain <cpoulain@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:24:28 by cpoulain          #+#    #+#             */
/*   Updated: 2025/11/25 13:41:03 by cpoulain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linker.h"

void   exit_ping(t_ping *ping, byte help)
{
    int exit_code = (ping->parser.last_error != ARGPARSE_OK && !help ? ping->parser.last_error : 0);
    
    exit_code ? print_argparse_error(&ping->parser) : print_usage(&ping->parser);
    free_arg_parser(&ping->parser);
    exit(exit_code);
}