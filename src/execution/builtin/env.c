/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:50:26 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/15 16:31:18 by hbousset         ###   ########.fr       */
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
		return ;
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
		new->content = NULL;
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

int	builtin_env(t_cmd *cmd)
{
	t_env	*current;

	if (cmd->av[1])
	{
		ft_error("env", "No arguments or options allowed", cmd->gc);
		return (1);
	}
	if (!cmd->env)
		return (ft_error("env", "Environments are not set", cmd->gc));
	current = cmd->env;
	while (current)
	{
		if (write(STDOUT_FILENO, "", 0) == -1)
			return (125);
		printf("%s=%s\n", current->var, current->content);
		current = current->next;
	}
	return (0);
}
