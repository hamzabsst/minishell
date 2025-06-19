/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:53:59 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/16 17:16:07 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_env(t_expand *head)
{
	while (head != NULL)
	{
		printf("%s=%s\n", head->var, head->content_of_var);
		head = head->next;
	}
}
void	add_expand_back(t_expand **head, t_expand *new)
{
	t_expand *tmp;
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

t_expand	*allocate_expand(t_cmd *cmd, char *var, char *content_of_var)
{
	t_expand *new;

	new = ft_malloc(cmd->collector ,sizeof(t_expand));
	if (!new)
		return (NULL);
	new->var = our_strdup(cmd->collector, var);
	new->content_of_var = our_strdup(cmd->collector, content_of_var);
	new->next = NULL;
	return (new);
}

t_expand	*copy_from_env(t_cmd *cmd, char **env)
{
	t_expand *head;
	t_expand *new;
	int i = 0;
	int j;
	int k;
	char **copy;

	head = NULL;
	i = 0;
	copy = ft_malloc(cmd->collector, sizeof(char *) * 3);
	if(!copy)
		return (NULL);
	while (env[i])
	{
		k = 0;
		j = 0;
		while (env[i][j])
		{
			while (env[i][j] != '=')
				j++;
			copy[0] = ft_substr(env[i], k, j - k);
			k = j + 1;
			while(env[i][j] != '\0')
				j++;
			copy[1] = ft_substr(env[i], k, (j - k + 1));
		}
		copy[2] = NULL;
		new = allocate_expand(cmd, copy[0], copy[1]);
		add_expand_back(&head, new);
		i++;
	}
	//print_env(head);
	return (head);
}
char	*get_from_env(t_cmd *cmd ,t_expand *env, char *str)
{
	char *word;
	int i;
	int j;

	i = 1;
	j = i;
	while (ft_isalnum(str[j]) == 0 || str[j] != '_')
		j++;
	word = ft_substr(str, i, (j - i));
	while (env->var)
	{
		if (ft_strcmp(word, env->var) == 0)
		{
			word = env->content_of_var;
			return (word);
		}
		env = env->next;
	}
	return(our_strdup(cmd->collector, ""));
}
