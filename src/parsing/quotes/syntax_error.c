/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:57:03 by abchaman          #+#    #+#             */
/*   Updated: 2025/07/04 15:47:49 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_redir(t_token *tkn, t_token *prv, t_mem *gc, const char *op)
{
	t_token	*curr;

	curr = tkn;
	if (ft_strcmp(curr->content, op) == 0)
	{
		if (prv && ft_strcmp(prv->type, "DELIMITER") == 0 && curr->next)
			return (0);
		if (!curr->next)
		{
			our_error("syntax error near unexpected token `newline'\n");
			return (1);
		}
		if (ft_strcmp(curr->next->type, "WORD") != 0)
		{
			ft_error("syntax error near unexpected token ",
				curr->next->content, "\n", gc);
			return (1);
		}
	}
	return (0);
}

static int	check_heredoc(t_token *tokens, t_mem *gc)
{
	t_token	*curr;

	curr = tokens;
	if (ft_strcmp(curr->content, "<<") == 0)
	{
		if (!curr->next)
		{
			our_error("syntax error near unexpected token `newline'\n");
			return (1);
		}
		if (!ft_strcmp(curr->next->type, "HEREDOC")
			|| (ft_strcmp(curr->next->type, "WORD")
				&& ft_strcmp(curr->next->type, "DELIMITER")))
		{
			ft_error("syntax error near unexpected token ",
				curr->next->content, "\n", gc);
			return (1);
		}
	}
	return (0);
}

static int	check_append(t_token *tokens, t_mem *gc)
{
	t_token	*curr;

	curr = tokens;
	if (ft_strcmp(curr->content, ">>>") == 0)
	{
		our_error("syntax error near unexpected token `>'\n");
		return (1);
	}
	if (ft_strcmp(curr->content, ">>") == 0)
	{
		if (curr->next == NULL)
		{
			our_error("syntax error near unexpected token `newline'\n");
			return (1);
		}
		else if (ft_strcmp(curr->next->type, "WORD") != 0)
		{
			ft_error("syntax error near unexpected token ",
				curr->next->content, "\n", gc);
			return (1);
		}
	}
	return (0);
}

static int	check_pipe(t_token *tokens, t_token *prev)
{
	t_token	*curr;

	curr = tokens;
	if (ft_strcmp(curr->content, "|") == 0)
	{
		if (!prev || !curr->next || ft_strcmp(curr->next->content, "|") == 0)
		{
			our_error("syntax error near unexpected token `|`\n");
			return (1);
		}
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
		if (check_heredoc(curr, data->gc)
			|| check_append(curr, data->gc)
			|| check_redir(curr, prev, data->gc, ">")
			|| check_redir(curr, prev, data->gc, "<")
			|| check_pipe(curr, prev)
			|| check_quotes_syntax(curr))
			return (1);
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
