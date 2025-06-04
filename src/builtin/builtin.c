/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:21:34 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/04 12:09:46 by hbousset         ###   ########.fr       */
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

static int	builtin_env(char **av, char **env)
{
	int	i;

	if (av[1])
		return (ft_perror("env: No arguments or options allowed\n"));
	if (!env)
		return (ft_perror("command not found: env\n"));
	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
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

int	exec_builtin(t_cmd *cmd, char ***env, t_mem *collector)
{
	if (!ft_strcmp(cmd->av[0], "echo"))
		return (builtin_echo(cmd->av));
	else if (!ft_strcmp(cmd->av[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->av[0], "cd"))
		return (builtin_cd(cmd->av, env, collector));
	else if (!ft_strcmp(cmd->av[0], "env"))
		return (builtin_env(cmd->av, *env));
	else if (!ft_strcmp(cmd->av[0], "unset"))
		return (builtin_unset(cmd->av, env, collector));
	else if (!ft_strcmp(cmd->av[0], "exit"))
		return (builtin_exit(cmd->av, env, collector));
	else if (!ft_strcmp(cmd->av[0], "export"))
		return (builtin_export(cmd->av, env, collector));
	return (1);
}
