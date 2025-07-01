/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:02:31 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/01 15:31:18 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*find_in_paths(char *av, char **paths, t_mem *gc)
{
	char	*full;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		full = ft_malloc(gc, ft_strlen(paths[i]) + ft_strlen(av) + 2);
		if (!full)
			return (NULL);
		ft_strcpy(full, paths[i]);
		ft_strcat(full, "/");
		ft_strcat(full, av);
		if (access(full, F_OK) == 0)
		{
			if (access(full, X_OK) == 0)
				return (our_strdup(gc, full));
			else
				return (our_strdup(gc, full));
		}
		i++;
	}
	return (NULL);
}

static char	*handle_absolute_path(t_cmd *cmd)
{
	if (access(cmd->av[0], F_OK) != 0)
	{
		ft_perror(cmd->av[0], ": No such file or directory\n", "\n", cmd->gc);
		cmd->exit_code = 127;
		return (NULL);
	}
	if (access(cmd->av[0], X_OK) == 0)
		return (our_strdup(cmd->gc, cmd->av[0]));
	else
	{
		ft_perror(cmd->av[0], ": Permission denied", "\n", cmd->gc);
		cmd->exit_code = 126;
		return (NULL);
	}
}

static char	*get_cmd_path(t_cmd *cmd, char **env)
{
	char	**paths;
	char	*resolved;

	if (!cmd || !cmd->av || !cmd->av[0])
		return (NULL);
	if (ft_strchr(cmd->av[0], '/'))
		return (handle_absolute_path(cmd));
	paths = get_path(env);
	if (!paths)
	{
		ft_perror(cmd->av[0], ": command not found", "\n", cmd->gc);
		return (cmd->exit_code = 127, NULL);
	}
	resolved = find_in_paths(cmd->av[0], paths, cmd->gc);
	ft_free(paths);
	if (!resolved)
	{
		ft_perror(cmd->av[0], ": command not found", "\n", cmd->gc);
		return (cmd->exit_code = 127, NULL);
	}
	if (access(resolved, X_OK) != 0)
	{
		ft_perror(cmd->av[0], ": Permission denied", "\n", cmd->gc);
		return (cmd->exit_code = 126, NULL);
	}
	return (resolved);
}

void	exec_cmd(t_cmd *cmd)
{
	char	*path;
	char	**env;
	int		exit_code;

	env = env_to_array(cmd);
	if (!cmd->av || !cmd->av[0])
		(ft_free_all(cmd->gc), exit(0));
	path = get_cmd_path(cmd, env);
	if (!path)
	{
		exit_code = cmd->exit_code;
		(ft_free_all(cmd->gc), exit(exit_code));
	}
	execve(path, cmd->av, env);
	perror("execve");
	ft_free_all(cmd->gc);
	exit(126);
}
