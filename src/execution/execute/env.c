/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:50:26 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/27 18:30:39 by hbousset         ###   ########.fr       */
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
	t_env		*head;
	t_env		*new;
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

static int	count_env_vars(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *env_var, void *gc)
{
	char	*temp;
	char	*result;

	if (env_var->content && ft_strlen(env_var->content) > 0)
	{
		temp = our_strjoin(gc, env_var->var, "=");
		if (!temp)
			return (NULL);
		result = our_strjoin(gc, temp, env_var->content);
		return (result);
	}
	else
		return (our_strdup(gc, env_var->var));
}

char	**env_to_array(t_cmd *cmd)
{
	t_env	*current;
	char	**array;
	int		count;
	int		i;

	count = count_env_vars(cmd->env);
	array = ft_malloc(cmd->gc, sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	current = cmd->env;
	i = 0;
	while (current && i < count)
	{
		array[i] = create_env_string(current, cmd->gc);
		if (!array[i])
			return (NULL);
		current = current->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
