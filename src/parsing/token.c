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

//? debugging
// static void	print_tokens(t_token *tokens)
// {
// 	int i = 0;
// 	while (tokens)
// 	{
// 		ft_printf("Token[%d]: %s Type: %s\n", i, tokens->content, tokens->type);
// 		tokens = tokens->next;
// 		i++;
// 	}
// }

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

static t_token	*allocate_token(t_cmd *cmd, char *content, const char *type)
{
	t_token	*new;

	new = ft_malloc(cmd->gc ,sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = our_strdup(cmd->gc, content);
	new->type = our_strdup(cmd->gc, type);
	new->next = NULL;
	return (new);
}

t_token	*tokenize(t_cmd *cmd, char **tokens)
{
	t_token		*head;
	t_token 	*new;
	const char	*type;
	int			i;

	i = 0;
	head = NULL;
	while (tokens[i])
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
		new = allocate_token(cmd, tokens[i], type);
		add_token_back(&head, new);
		i++;
	}
	return (head);
}

