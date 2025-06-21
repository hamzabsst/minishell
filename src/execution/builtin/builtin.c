/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:21:34 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 11:37:13 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_pwd(void)
{
	char	*cwd;

	cwd = malloc(1024);
	if (cwd != NULL)
		getcwd(cwd, 1024);
	if (!cwd)
		return (perror("pwd"), 1);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

static int	builtin_env(t_cmd *cmd)
{
	if (cmd->av[1])
		return (ft_perror("env: No arguments or options allowed\n"));
	if (!cmd->env)
		return (ft_perror("command not found: env\n"));
	while (cmd->env->next)
	{
		printf("%s", cmd->env->var);
		printf("=");
		printf("%s\n", cmd->env->content);
		cmd->env = cmd->env->next;
	}
	return (0);
}

int	builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
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
