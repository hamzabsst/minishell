/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:36 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/19 14:21:35 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	main(void)
{
	t_command	cmd1 = {
		.argv = (char *[]){"ls", "-l", NULL},
		.infile = NULL,
		.outfile = "out.txt",
		.append = 0,
		.heredoc = 0,
		.delimiter = NULL,
		.next = NULL
	};
}
