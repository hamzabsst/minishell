/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:35:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/16 18:43:13 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cwd(t_mem *gc)
{
	char	*cwd;

	cwd = ft_malloc(gc, PATH_MAX);
	if (!cwd)
		return (NULL);
	if (!getcwd(cwd, PATH_MAX))
	{
		if (chdir("/home") == 0)
		{
			ft_strcpy(cwd, "/home");
			return (cwd);
		}
		return (NULL);
	}
	return (cwd);
}

char	*ft_getenv(t_env *env, const char *key)
{
	t_env	*current;

	if (!env || !key)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->var && !ft_strcmp(current->var, key))
			return (current->content);
		current = current->next;
	}
	return (NULL);
}

static int	update_env(t_cmd *cmd, const char *key, const char *value)
{
	t_env	*current;

	if (!cmd->env || !key || !cmd->gc)
		return (1);
	current = cmd->env;
	while (current)
	{
		if (current->var && !ft_strcmp(current->var, key))
		{
			if (value)
				current->content = our_strdup(cmd->gc, value);
			else
				current->content = our_strdup(cmd->gc, "");
			if (!current->content)
				return (1);
			return (0);
		}
		current = current->next;
	}
	return (0);
}

static int	handle_path(t_cmd *cmd)
{
	if (!cmd->av[1])
		return (0);
	else if (*cmd->av[1] == '~' || *cmd->av[1] == '-')
		return (our_error("cd: only absolute or relative paths are allowed\n"));
	else if (cmd->av[2])
		return (our_error("cd: too many arguments\n"));
	return (0);
}

//export home= still dont behave like bash
int	builtin_cd(t_cmd *cmd)
{
	char	*oldpwd;
	char	*newpwd;
	char	*path;

	oldpwd = get_cwd(cmd->gc);
	if (!oldpwd)
		return (1);
	if (handle_path(cmd))
		return (1);
	if (!cmd->av[1] || !*cmd->av[1])
	{
		path = ft_getenv(cmd->env, "HOME");
		if (!path)
			return (our_error("cd: HOME not set\n"));
	}
	else
		path = cmd->av[1];
	if (chdir(path) == -1)
		return (ft_error(path, strerror(errno), cmd->gc));
	if (update_env(cmd, "OLDPWD", oldpwd) != 0)
		return (1);
	newpwd = get_cwd(cmd->gc);
	if (!newpwd)
		return (1);
	return (update_env(cmd, "PWD", newpwd));
}
