/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:36 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/19 15:16:35 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
