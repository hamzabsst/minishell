/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:11:48 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/03 17:42:35 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	var_exists(t_env *env, const char *key)
{
	t_env	*current;

	if (!env || !key)
		return (0);
	current = env;
	while (current)
	{
		if (current->var && !ft_strcmp(current->var, key))
			return (1);
		current = current->next;
	}
	return (0);
}

static int	update_env_append(t_cmd *cmd, char *key, char *value)
{
	t_env	*current;
	char	*old;
	char	*new;

	if (!cmd->env || !key || !value || !cmd->gc)
		return (1);
	current = cmd->env;
	while (current)
	{
		if (current->var && !ft_strcmp(current->var, key))
		{
			old = current->content;
			if (old)
				new = ft_strjoin(old, value);
			else
				new = our_strdup(cmd->gc, value);
			if (!new)
				return (1);
			current->content = new;
			return (0);
		}
		current = current->next;
	}
	return (update_env(cmd, key, value));
}

static char	*find_key(const char *arg, t_mem *gc)
{
	int	len;

	len = 0;
	while (arg[len] && arg[len] != '='
		&& !(arg[len] == '+' && arg[len + 1] == '='))
		len++;
	return (our_substr(arg, 0, len, gc));
}

void	process_av(t_cmd *cmd, char *arg)
{
	char	*key;
	char	*equal;

	key = find_key(arg, cmd->gc);
	if (!key)
		return ;
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		if (equal > arg && *(equal - 1) == '+')
			update_env_append(cmd, key, equal + 1);
		else
			update_env(cmd, key, equal + 1);
	}
	else if (!var_exists(cmd->env, key))
		update_env(cmd, key, "");
}
