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

static void	add_token_back(t_token **head, t_token *new)
{
	t_token *tmp;
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

	new = ft_malloc(gc ,sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = our_strdup(gc, content);
	new->type = our_strdup(gc, type);
	new->next = NULL;
	return (new);
}

t_token	*tokenize(t_mem *gc, char **tokens)
{
	const char	*type;
	t_token		*head;
	t_token 	*new;
	int			i;

	(i = -1, head = NULL);
	while (tokens[++i])
	{
		type = "WORD";
		if (ft_strcmp(tokens[i], "|") == 0)
			type = "PIPE";
		else if (ft_strcmp(tokens[i], ">") == 0)
			type = "REDIRECTION_OUT";
		else if (ft_strcmp(tokens[i], "<") == 0)
			type = "REDIRECTION_IN";
		else if(ft_strcmp(tokens[i], ">>") == 0)
			type = "APPEND";
		else if (ft_strcmp(tokens[i], "<<") == 0)
			type = "HEREDOC";
		else if(i > 0 && ft_strcmp(tokens[i - 1], "<<") == 0)
			type = "DELIMITER";
		new = allocate_token(gc, tokens[i], type);
		add_token_back(&head, new);
	}
	return (head);
}

