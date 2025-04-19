/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/19 22:00:24 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int main(int ac, char **av, char **env)
{

	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		exit(1);
	}
	t_cmd cmd = {
		.argv = (char *[]){"pwd", NULL},
		.infile = NULL,
		.outfile = NULL,
		.append = 0,
		.heredoc = 0,
		.delimiter = NULL,
		.next = NULL
	};

	if (builtin(cmd.argv[0]) && !cmd.next)
		return (exec_builtin(&cmd, env));
	if (*av[5] == 5)
		return (1);
	return (0);
}

