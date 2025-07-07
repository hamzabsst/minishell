/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:57:03 by abchaman          #+#    #+#             */
/*   Updated: 2025/07/07 15:19:19 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_redir(t_token *token, t_token *prev, const char *op)
{
	t_token	*curr;

	curr = token;
	if (!ft_strcmp(curr->content, op))
	{
		if (prev && !ft_strcmp(prev->type, "DELIMITER") && curr->next)
			return (0);
		if (!curr->next || ft_strcmp(curr->next->type, "WORD"))
			return (1);
	}
	return (0);
}

static int	check_heredoc(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	if (ft_strcmp(curr->content, "<<") == 0)
	{
		if (!curr->next)
			return (1);
		if (!ft_strcmp(curr->next->type, "HEREDOC")
			|| (ft_strcmp(curr->next->type, "WORD")
				&& ft_strcmp(curr->next->type, "DELIMITER")))
		{
			return (1);
		}
	}
	return (0);
}

static int	check_append(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	if (!ft_strcmp(curr->content, ">>"))
	{
		if (!curr->next || ft_strcmp(curr->next->type, "WORD"))
			return (1);
	}
	return (0);
}

static int	check_pipe(t_token *tokens, t_token *prev)
{
	t_token	*curr;

	curr = tokens;
	if (!ft_strcmp(curr->content, "|"))
	{
		if (!prev || !curr->next || !ft_strcmp(curr->next->content, "|"))
			return (1);
	}
	return (0);
}

int	check_syntax_error(t_parse *data)
{
	t_token	*prev;
	t_token	*curr;

	prev = NULL;
	curr = data->tokens;
	while (curr)
	{
		if (check_heredoc(curr)
			|| check_append(curr)
			|| check_redir(curr, prev, ">")
			|| check_redir(curr, prev, "<")
			|| check_pipe(curr, prev)
			|| check_quotes_syntax(curr))
			return (our_error("syntax error\n"));
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
