/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 10:56:51 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_cmd	*cmd = NULL;
	char	**g_env;

	//////////////init
	t_cmd c1 = {
		.argv = (char *[]){"ls", NULL},
		.infile = NULL,
		.outfile = NULL,
		.append = 0,
		.next = NULL
	};
	t_cmd c2 = {
		.argv = (char *[]){"grep", ".c", NULL},
		.infile = NULL,
		.outfile = "result.txt",
		.append = 0,
		.next = NULL
	};
	c1.next = &c2;
	//////////////////
	(void)av;
	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		exit(1);
	}
	g_env = dup_env(env);
	if (!cmd->next && builtin(cmd->argv[0]))
		return exec_builtin(cmd, &g_env);
	else
		exec_pipeline(&c1, g_env);
}
