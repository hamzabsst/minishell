/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:50:26 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/25 11:47:29 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_var_back(t_env **head, t_env *new)
{
	t_env	*tmp;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return;
	}
	tmp = *head;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	tmp->next = new;
}

t_env	*allocate_var(const char *var, const char *content, t_mem *gc)
{
	t_env	*new;

	if (!var || !gc)
		return (NULL);
	new = ft_malloc(gc, sizeof(t_env));
	if (!new)
		return (NULL);
	new->var = our_strdup(gc, var);
	if (!new->var)
		return (NULL);
	if (content)
		new->content = our_strdup(gc, content);
	else
		new->content = our_strdup(gc, "");
	if (!new->content)
		return (NULL);
	new->next = NULL;
	return (new);
}

static int	parse_env(const char *env, char **var, char **value, t_mem *gc)
{
	char	*equals_pos;
	size_t	var_len;
	size_t	value_len;

	if (!env || !var || !value || !gc)
		return (0);
	equals_pos = ft_strchr(env, '=');
	if (!equals_pos)
		return (0);
	var_len = equals_pos - env;
	value_len = ft_strlen(equals_pos + 1);
	*var = our_substr(env, 0, var_len, gc);
	if (!*var)
		return (0);
	*value = our_substr(env, var_len + 1, value_len, gc);
	if (!*value)
		return (0);
	return (1);
}

t_env	*dup_env(char **env, t_mem *gc)
{
	t_env	*head;
	t_env	*new;
	char		*var;
	char		*value;
	int			i;

	if (!env || !gc)
		return (NULL);
	head = NULL;
	i = 0;
	while (env[i])
	{
		var = NULL;
		value = NULL;
		if (parse_env(env[i], &var, &value, gc))
		{
			new = allocate_var(var, value, gc);
			if (new)
				add_var_back(&head, new);
		}
		i++;
	}
	return (head);
}

char **env_to_array(t_cmd *cmd)
{
	t_env	*current;
	char	**array;
	char	*temp;
	int		count;
	int		i;

	count = 0;
	current = cmd->env;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = ft_malloc(cmd->gc, sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	current = cmd->env;
	i = 0;
	while (current && i < count)
	{
		if (current->content && ft_strlen(current->content) > 0)
		{
			temp = our_strjoin(cmd->gc, current->var, "=");
			if (temp)
				array[i] = our_strjoin(cmd->gc, temp, current->content);
		}
		else
			array[i] = our_strdup(cmd->gc, current->var);
		if (!array[i])
			return (NULL);
		current = current->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
