/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:39:41 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 12:04:23 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*our_substr(char const *s, unsigned int start, size_t len, t_mem *gc)
{
	char	*substr;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (our_strdup(gc, ""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	substr = ft_malloc(gc, len * sizeof(char) + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (len > i && s[start + i])
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

int	update_env_append(t_cmd *cmd, char *key, char *value)
{
	char		*old_value;
	char		*new_value;
	t_expand	*current;

	if (!cmd->env || !key || !value || !cmd->gc)
		return (1);
	current = cmd->env;
	while (current)
	{
		if (current->var && ft_strcmp(current->var, key) == 0)
		{
			old_value = current->content;
			if (old_value)
				new_value = ft_strjoin(old_value, value);
			else
				new_value = our_strdup(cmd->gc, value);
			if (!new_value)
				return (1);
			current->content = new_value;
			return (0);
		}
		current = current->next;
	}
	return (update_env(cmd, key, value));
}
void	add_expand_back(t_expand **head, t_expand *new)
{
	t_expand	*tmp;

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

t_expand	*allocate_expand(char *var, char *content, t_mem *gc)
{
	t_expand	*new;

	if (!var || !gc)
		return (NULL);
	new = ft_malloc(gc, sizeof(t_expand));
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

int	parse_env(const char *env, char **var, char **value, t_mem *gc)
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

t_expand	*dup_env(char **env, t_mem *gc)
{
	t_expand	*head;
	t_expand	*new;
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
			new = allocate_expand(var, value, gc);
			if (new)
				add_expand_back(&head, new);
		}
		i++;
	}
	return (head);
}

char	*extract_var_name(const char *str, t_mem *gc)
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
//? i dont use this yet
char	*get_from_env(t_expand *env, char *str, t_mem *gc)
{
	char		*var_name;
	t_expand	*current;

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
}

char *ft_getenv(t_expand *env, const char *key)
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

int	ft_perror(char *msg)
{
	if (!msg)
		return (1);
	ft_putstr_fd(msg, 2);
	return (1);
}

