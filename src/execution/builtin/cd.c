/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:35:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 18:49:53 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *ft_getenv(t_expand *env, const char *key)
{
	t_expand	*current;
	if (!env || !key)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->var && ft_strcmp(current->var, key) == 0)
			return (current->content);
		current = current->next;
	}
	return (NULL);
}

static int	handle_path(const char *arg)
{
	if (!arg || !*arg)
		return (0);
	if (arg[0] == '~' || arg[0] == '-')
	{
		ft_perror("cd: only absolute or relative paths are allowed\n");
		return (-1);
	}
	return (1);
}

int update_env(t_cmd *cmd, char *key, char *value)
{
	t_expand	*current;
	t_expand	*new_var;

	if (!cmd->env || !key || !cmd->gc)
		return (1);
	current = cmd->env;
	while (current)
	{
		if (current->var && ft_strcmp(current->var, key) == 0)
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

int	builtin_cd(t_cmd *cmd)
{
	char	*oldpwd;
	char	*newpwd;
	int		ret;
	char	*path;

	oldpwd = malloc(1024);
	if (oldpwd != NULL)
		getcwd(oldpwd, 1024);
	if (!oldpwd)
		return (perror("cd: getcwd"), 1);
	if (handle_path(cmd->av[1]) == -1)
		return (free(oldpwd), 1);
	if (!cmd->av[1] || !*cmd->av[1])
		path = ft_getenv(cmd->env, "HOME");
	else
		path = cmd->av[1];
	if (chdir(path) == -1)
		return (ft_perror("cd: "), perror(path), free(oldpwd), 1);
	if (update_env(cmd, "OLDPWD", oldpwd) != 0)
		return (free(oldpwd), 1);
	free(oldpwd);
	newpwd = malloc(1024);
	if (newpwd != NULL)
		getcwd(newpwd, 1024);
	if (!newpwd)
		return (perror("cd: getcwd"), 1);
	ret = update_env(cmd, "PWD", newpwd);
	return (free(newpwd), ret);
}
