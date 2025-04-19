/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:02:11 by abchaman          #+#    #+#             */
/*   Updated: 2025/04/19 21:51:09 by abchaman         ###   ########.fr       */
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
	t_cmd cmd = {
		.argv = (char *[]){"echo", "-n", "hello", "qwd", NULL},
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

