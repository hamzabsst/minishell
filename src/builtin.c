/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:21:34 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 10:23:50 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

int	builtin_echo(char **argv)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	if (argv[i] && !ft_strcmp(argv[i], "-n"))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
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

int	builtin_env(char **argv, char **envp)
{
	int	i;

	if (argv[1])
	{
		write(2, "env: No arguments or options allowed\n", 39);
		return (1);
	}
	i = 0;
	if (!envp)
	{
		printf("%s\n","command not found: env");
		return (1);
	}
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			printf("%s\n", envp[i]);
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
	if (!ft_strcmp(cmd->argv[0], "echo"))
		return (builtin_echo(cmd->argv));
	else if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->argv[0], "cd"))
		return (builtin_cd(cmd->argv, env));
	else if (!ft_strcmp(cmd->argv[0], "env"))
		return (builtin_env(cmd->argv, *env));
	else if (!ft_strcmp(cmd->argv[0], "unset"))
		return (builtin_unset(cmd->argv, env));
	else if (!ft_strcmp(cmd->argv[0], "exit"))
		return (builtin_exit(cmd->argv, env));
	else if(!ft_strcmp(cmd->argv[0], "export"))
		return (builtin_export(cmd->argv, *env));
	return (1);
}
