/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:21:34 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/25 17:14:48 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_perror("pwd: getcwd failed\n"));
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

static int	builtin_env(t_cmd *cmd)
{
	t_env	*current;

	if (cmd->av[1])
		return (ft_perror("env: No arguments or options allowed\n"));
	if (!cmd->env)
		return (ft_perror("command not found: env\n"));
	current = cmd->env;
	while (current)
	{
		printf("%s=%s\n", current->var, current->content);
		current = current->next;
	}
	return (0);
}

int	builtin(char *av)
{
	if (!av)
		return (0);
	return (!ft_strcmp(av, "echo")
		|| !ft_strcmp(av, "cd")
		|| !ft_strcmp(av, "pwd")
		|| !ft_strcmp(av, "export")
		|| !ft_strcmp(av, "unset")
		|| !ft_strcmp(av, "env")
		|| !ft_strcmp(av, "exit"));
}

int	exec_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->av[0], "echo"))
		return (builtin_echo(cmd));
	else if (!ft_strcmp(cmd->av[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->av[0], "cd"))
		return (builtin_cd(cmd));
	else if (!ft_strcmp(cmd->av[0], "env"))
		return (builtin_env(cmd));
	else if (!ft_strcmp(cmd->av[0], "unset"))
		return (builtin_unset(cmd));
	else if (!ft_strcmp(cmd->av[0], "exit"))
		return (builtin_exit(cmd));
	else if (!ft_strcmp(cmd->av[0], "export"))
		return (builtin_export(cmd));
	return (1);
}
