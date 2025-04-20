/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/20 11:21:50 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main(int ac, char **av, char **env)
{
	t_cmd	cmd;

	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		exit(1);
	}
	cmd.argv = (char *[]){"cd","-", NULL};
	cmd.next = NULL;
/* 	if (builtin(cmd.argv[0]) && !cmd.next)
		return (exec_builtin(&cmd, env)); */
	exec_builtin(&cmd, env);
	cmd.argv = (char *[]){"pwd", NULL};
	exec_builtin(&cmd, env);
	if (*av[5] == 5)
		return (1);
	return (0);
}

