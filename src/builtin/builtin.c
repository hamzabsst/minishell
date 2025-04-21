/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:21:34 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 13:47:49 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	builtin_echo(char **av)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	if (av[i] && !ft_strcmp(av[i], "-n"))
	{
		newline = 0;
		i++;
	}
	while (av[i])
	{
		printf("%s", av[i]);
		if (av[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	builtin_env(char **av, char **env)
{
	int	i;

	if (av[1])
	{
		write(2, "env: No arguments or options allowed\n", 39);
		return (1);
	}
	i = 0;
	if (!env)
	{
		printf("%s\n","command not found: env");
		return (1);
	}
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

int	builtin_exit(char **argv, char ***env)
{
	(void)argv;
	write(1, "exit\n", 5);
	ft_free(*env);
	exit(0);
}

int	exec_builtin(t_cmd *cmd, char ***env)
{
	if (!ft_strcmp(cmd->av[0], "echo"))
		return (builtin_echo(cmd->av));
	else if (!ft_strcmp(cmd->av[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->av[0], "cd"))
		return (builtin_cd(cmd->av, env));
	else if (!ft_strcmp(cmd->av[0], "env"))
		return (builtin_env(cmd->av, *env));
	else if (!ft_strcmp(cmd->av[0], "unset"))
		return (builtin_unset(cmd->av, env));
	else if (!ft_strcmp(cmd->av[0], "exit"))
		return (builtin_exit(cmd->av, env));
	else if(!ft_strcmp(cmd->av[0], "export"))
		return (builtin_export(cmd->av, *env));
	return (1);
}
