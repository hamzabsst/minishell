/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 10:42:34 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_cmd	cmd;
	char	**g_env;

	(void)av;
	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		exit(1);
	}
	g_env = dup_env(env);
	cmd.argv = (char *[]){"export", NULL};
	exec_builtin(&cmd, &g_env);
	// cmd.argv = (char *[]){"env", NULL};
	// if (execve("env", cmd.argv, g_env) == -1)
	// 	perror("env");
}
