/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:03:59 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/01 14:03:59 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_token **start, char *content, const char *type, t_mem *gc)
{
	t_token	*new;
	t_token	*current;

	new = ft_malloc(gc, sizeof(t_token));
	if (!new)
		return ;
	new->content = content;
	new->type = our_strdup(gc, type);
	new->next = NULL;
	if (!*start)
	{
		*start = new;
		return ;
	}
	current = *start;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	repl_token(t_token **tokens, t_token *curr, t_token *new, t_token *end)
{
	t_token	*tmp;

	tmp = *tokens;
	if (*tokens == curr)
		*tokens = new;
	else
	{
		while (tmp && tmp->next != curr)
			tmp = tmp->next;
		if (tmp)
			tmp->next = new;
	}
	if (end)
		end->next = curr->next;
}

static void	add_token_back(t_token **head, t_token *new)
{
	t_token	*tmp;

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

static t_token	*allocate_token(t_mem *gc, char *content, const char *type)
{
	t_token	*new;

	new = ft_malloc(gc, sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = our_strdup(gc, content);
	new->type = our_strdup(gc, type);
	new->next = NULL;
	return (new);
}

t_token	*tokenize(t_parse *data)
{
	const char	*type;
	t_token		*head;
	int			i;

	head = NULL;
	i = -1;
	while (data->splited[++i])
	{
		type = "WORD";
		if (!ft_strcmp(data->splited[i], "|"))
			type = "PIPE";
		else if (!ft_strcmp(data->splited[i], ">"))
			type = "REDIRECTION_OUT";
		else if (!ft_strcmp(data->splited[i], "<"))
			type = "REDIRECTION_IN";
		else if (!ft_strcmp(data->splited[i], ">>"))
			type = "APPEND";
		else if (!ft_strcmp(data->splited[i], "<<"))
			type = "HEREDOC";
		else if (i > 0 && !ft_strcmp(data->splited[i - 1], "<<"))
			type = "DELIMITER";
		add_token_back(&head, allocate_token(data->gc, data->splited[i], type));
	}
	return (head);
}
