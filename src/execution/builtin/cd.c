/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:35:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/07 15:08:48 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	update_env(t_cmd *cmd, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_var;

	if (!cmd->env || !key || !cmd->gc)
		return (1);
	current = cmd->env;
	while (current)
	{
		if (current->var && !ft_strcmp(current->var, key))
		{
			if (current->content)
				current->content = our_strdup(cmd->gc, value);
			else
				current->content = our_strdup(cmd->gc, "");
			if (!current->content)
				return (1);
			return (0);
		}
		current = current->next;
	}
	new_var = allocate_var(key, value, cmd->gc);
	if (!new_var)
		return (1);
	add_var_back(&cmd->env, new_var);
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
	else
		return (0);
}

int	builtin_cd(t_cmd *cmd)
{
	char	*oldpwd;
	char	*newpwd;
	int		ret;
	char	*path;

	oldpwd = malloc(1024);
	if (oldpwd != NULL)
		getcwd(oldpwd, 1024);
	if (handle_path(cmd))
		return (free(oldpwd), 1);
	if (!cmd->av[1] || !*cmd->av[1])
		path = ft_getenv(cmd->env, "HOME");
	else
		path = cmd->av[1];
	if (!path)
		return (free(oldpwd), 0);
	if (chdir(path) == -1)
		return (free(oldpwd), ft_error(path, strerror(errno), cmd->gc));
	if (update_env(cmd, "OLDPWD", oldpwd) != 0)
		return (free(oldpwd), 1);
	free(oldpwd);
	newpwd = malloc(1024);
	if (newpwd != NULL)
		getcwd(newpwd, 1024);
	ret = update_env(cmd, "PWD", newpwd);
	return (free(newpwd), ret);
}
