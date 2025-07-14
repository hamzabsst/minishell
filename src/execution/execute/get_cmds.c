/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:02:31 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/13 21:19:34 by hbousset         ###   ########.fr       */
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
	struct stat	file;
	char		*full;
	char		*path;
	int			i;

	i = -1;
	path = NULL;
	while (paths && paths[++i])
	{
		full = ft_malloc(gc, ft_strlen(paths[i]) + ft_strlen(av) + 2);
		if (!full)
			return (NULL);
		ft_strcpy(full, paths[i]);
		(ft_strcat(full, "/"), ft_strcat(full, av));
		if (!access(full, F_OK))
		{
			if (!stat(full, &file) && S_ISDIR(file.st_mode))
				continue ;
			if (!access(full, X_OK))
				return (our_strdup(gc, full));
			if (path == NULL)
				path = full;
		}
	}
	return (path);
}

static char	*handle_absolute_path(t_cmd *cmd)
{
	struct stat	file_stat;

	if (!stat(cmd->av[0], &file_stat) && S_ISDIR(file_stat.st_mode))
	{
		ft_error(cmd->av[0], "Is a directory", cmd->gc);
		cmd->exit_code = 126;
		return (NULL);
	}
	if (access(cmd->av[0], F_OK))
	{
		ft_error(cmd->av[0], strerror(errno), cmd->gc);
		cmd->exit_code = 127;
		return (NULL);
	}
	if (!access(cmd->av[0], X_OK))
		return (our_strdup(cmd->gc, cmd->av[0]));
	else
	{
		ft_error(cmd->av[0], strerror(errno), cmd->gc);
		cmd->exit_code = 126;
		return (NULL);
	}
}

static char	*check_permission(t_cmd *cmd, char *resolved)
{
	struct stat	file;

	if (!stat(resolved, &file) && S_ISDIR(file.st_mode))
	{
		ft_error(cmd->av[0], "Is a directory", cmd->gc);
		cmd->exit_code = 126;
		return (NULL);
	}
	if (access(resolved, X_OK))
	{
		ft_error(cmd->av[0], strerror(errno), cmd->gc);
		cmd->exit_code = 126;
		return (NULL);
	}
	return (resolved);
}

char	*get_cmd_path(t_cmd *cmd, char **env)
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
		ft_error(cmd->av[0], "command not found", cmd->gc);
		cmd->exit_code = 127;
		return (NULL);
	}
	resolved = find_in_paths(cmd->av[0], paths, cmd->gc);
	ft_free(paths);
	if (!resolved)
	{
		ft_error(cmd->av[0], "command not found", cmd->gc);
		cmd->exit_code = 127;
		return (NULL);
	}
	return (check_permission(cmd, resolved));
}
