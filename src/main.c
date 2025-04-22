/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/22 11:30:29 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	**g_env;

	t_cmd cmd = {
		.av = (char *[]){"cat", NULL},
		.infile = "Makefile",
		.outfile = "out.txt",
		.append = 0,
		.heredoc = 0,
		.delimiter = NULL,
		.next = NULL
	};
	(void)av;
	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		exit(1);
	}
	g_env = dup_env(env);
	if (!cmd.next && builtin(cmd.av[0]))
		return exec_builtin(&cmd, &g_env);
	else
		exec_pipeline(&cmd, g_env);
}
