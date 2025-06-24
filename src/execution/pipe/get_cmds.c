/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:02:31 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/24 11:41:18 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//!!! handle sudo exit code , permission denied not command not found

void cleanup_child(t_mem *gc)
{
	if (gc)
		ft_free_all(gc);
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

static char	*find_in_paths(char *cmd, char **paths, t_mem *gc)
{
	char	*full;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		full = ft_malloc(gc, ft_strlen(paths[i]) + ft_strlen(cmd) + 2);
		if (!full)
			return (NULL);
		ft_strcpy(full, paths[i]);
		ft_strcat(full, "/");
		ft_strcat(full, cmd);
		if (access(full, X_OK) == 0)
			return (our_strdup(gc, full));
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
			return (our_strdup(cmd->gc, cmd->av[0]));
		ft_perror(cmd->av[0]);
		ft_perror(": Permission denied\n");
		exit(126);
	}
	paths = get_path(env);
	if (!paths)
		return (NULL);
	resolved = find_in_paths(cmd->av[0], paths, cmd->gc);
	ft_free(paths);
	return (resolved);
}

void	exec_cmd(t_cmd *cmd)
{
	char	*path;
	char	**env;

	env = env_to_array(cmd);
	if (!cmd->av || !cmd->av[0])
		(cleanup_child(cmd->gc), exit(0));
	path = get_cmd_path(cmd, env);
	if (!path)
	{
		ft_perror(cmd->av[0]);
		ft_perror(": command not found\n");
		(cleanup_child(cmd->gc), exit(127));
	}
	//cleanup_child(gc);
	execve(path, cmd->av, env);
	perror("execve");
	free(path);
	exit(1);
}
