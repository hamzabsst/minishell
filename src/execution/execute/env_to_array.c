/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 01:53:06 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/02 01:53:24 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
