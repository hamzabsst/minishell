/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 14:48:16 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	**g_env;

	//////////////init
 	t_cmd cmd= {
		.av = (char *[]){"ls", NULL},
		.infile = NULL,
		.outfile = NULL,
		.append = 0,
		.next = NULL
	};
 	t_cmd c2 = {
		.av = (char *[]){"wc", NULL},
		.infile = NULL,
		.outfile = "result.txt",
		.append = 0,
		.next = NULL
	};
	cmd.next = &c2;
	//////////////////
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
