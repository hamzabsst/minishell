/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:36 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/19 15:35:22 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int main(int ac, char **av, char **env)
{

	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments.\n", 36);
		exit(1);
	}
	t_command cmd = {
		.argv = (char *[]){"echo", "-n", "hello", "world", NULL},
		.infile = NULL,
		.outfile = NULL,
		.append = 0,
		.heredoc = 0,
		.delimiter = NULL,
		.next = NULL
	};

	if (is_builtin(cmd.argv[0]) && !cmd.next)
		return (exec_builtin(&cmd, env));
	if (*av[5] == 5)
		return (1);
	return (0);
}

