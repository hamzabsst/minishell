/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:50:26 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/22 14:17:23 by hbousset         ###   ########.fr       */
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

t_env	*allocate_var(char *var, char *content, t_mem *gc)
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
	{
		*var = NULL;
		return (0);
	}
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

//?? i dont use this yet
/* static char	*extract_var_name(const char *str, t_mem *gc)
{
	int	start;
	int	end;

	if (!str || str[0] != '$' || !gc)
		return (NULL);
	start = 1;
	end = start;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	if (end == start)
		return (NULL);
	return (our_substr(str, start, end - start, gc));
}

char	*get_from_env(t_env *env, char *str, t_mem *gc)
{
	char		*var_name;
	t_env	*current;

	if (!str || !env || !gc)
		return (our_strdup(gc, ""));
	var_name = extract_var_name(str, gc);
	if (!var_name)
		return (our_strdup(gc, ""));
	current = env;
	while (current)
	{
		if (current->var && ft_strcmp(var_name, current->var) == 0)
		{
			if (current->content)
				return (our_strdup(gc, current->content));
			else
				return (our_strdup(gc, ""));
		}
		current = current->next;
	}
	return (our_strdup(gc, ""));
} */

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
