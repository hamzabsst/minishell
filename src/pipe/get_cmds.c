/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:02:31 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/28 10:44:16 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_child(t_mem *collector)
{
	if (collector)
		ft_free_all(collector);
}

static char	**get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	return (ft_split(env[i] + 5, ':'));
}

static char	*find_in_paths(char *cmd, char **paths)
{
	char	*full;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		full = malloc(ft_strlen(paths[i]) + ft_strlen(cmd) + 2);
		if (!full)
			return (NULL);
		ft_strcpy(full, paths[i]);
		ft_strcat(full, "/");
		ft_strcat(full, cmd);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

static char	*get_cmd_path(t_cmd *cmd, char **env)
{
	char	**paths;
	char	*resolved;

	if (!cmd || !cmd->av || !cmd->av[0])
		return (NULL);
	if (ft_strchr(cmd->av[0], '/'))
	{
		if (access(cmd->av[0], X_OK) == 0)
			return (ft_strdup(cmd->av[0]));
		perror(cmd->av[0]);
		exit(126);
	}
	paths = get_path(env);
	if (!paths)
		return (NULL);
	resolved = find_in_paths(cmd->av[0], paths);
	ft_free(paths);
	return (resolved);
}

void	exec_cmd(t_cmd *cmd, char **env, t_mem *collector)
{
	char	*path;

	if (!cmd->av || !cmd->av[0])
		(cleanup_child(collector), exit(0));
	path = get_cmd_path(cmd, env);
	if (!path)
	{
		ft_putstr_fd(cmd->av[0], 2);
		ft_perror(": command not found\n");
		(cleanup_child(collector), exit(127));
	}
	//cleanup_child(collector);
	execve(path, cmd->av, env);
	perror("execve");
	free(path);
	exit(1);
}
