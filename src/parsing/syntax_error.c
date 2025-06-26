/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:57:03 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/25 21:24:26 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_heredoc(t_token *tokens, t_mem *gc)
{
	t_token	*curr;

	curr = tokens;
	if (ft_strcmp(curr->content, "<<") == 0)
	{
		if (curr->next && ft_strcmp(curr->next->type, "WORD") != 0
			&& ft_strcmp(curr->next->type, "DELIMITER") != 0)
		{
			ft_perror("syntax error near unexpected token ", curr->next->content, "\n", gc);
			return(1);
		}
		else if(curr->next && ft_strcmp(curr->next->type, "HEREDOC") == 0)
		{
			ft_perror("syntax error near unexpected token ", curr->next->content, "\n", gc);
			return (1);
		}
		else if (curr->next == NULL)
			return (our_perror("syntax error near unexpected token `newline'\n"));
	}
	return (0);
}

static int	check_append(t_token *tokens, t_mem *gc)
{
	t_token	*curr;

	curr = tokens;
	if (ft_strcmp(curr->content, ">>>") == 0)
		return (our_perror("syntax error near unexpected token `>'\n"));
	if (ft_strcmp(curr->content, ">>") == 0)
	{
		if (curr->next == NULL)
			return (our_perror("syntax error near unexpected token `newline'\n"));
		else if (ft_strcmp(curr->next->type, "WORD") != 0)
		{
			ft_perror("syntax error near unexpected token ", curr->next->content, "\n", gc);
			return (1);
		}
	}
	return (0);
}

static int	check_red_out(t_token *tokens, t_token *prev, t_mem *gc)
{
	t_token	*curr;

	curr = tokens;
	if (ft_strcmp(curr->content, ">") == 0)
	{
		if (prev != NULL)
		{
			if (ft_strcmp(prev->type, "DELIMITER") == 0 && curr->next != NULL)
				return (0);
		}
		else if (curr->next && ft_strcmp(curr->next->type, "WORD") != 0)
		{
			ft_perror("syntax error near unexpected token ", curr->next->content, "\n", gc);
			return (1);
		}
		else if (curr->next == NULL)
			return (our_perror("syntax error near unexpected token `newline'\n"));
	}
	return (0);
}

static int	check_red_in(t_token *tokens, t_token *prev, t_mem *gc)
{
	t_token *curr;

	curr = tokens;
	if (ft_strcmp(curr->content, "<") == 0)
	{
		if (prev != NULL)
		{
			if (ft_strcmp(prev->type, "DELIMITER") == 0 && curr->next != NULL)
				return (0);
		}
		if (curr->next && ft_strcmp(curr->next->type, "WORD") != 0)
		{
			ft_perror("syntax error near unexpected token ", curr->next->content, "\n", gc);
			return (1);
		}
		else if (curr->next == NULL)
			return (our_perror("syntax error near unexpected token `newline'\n"));
	}
	return (0);
}

static int check_pipe(t_token *tokens, t_token *prev)
{
	t_token	*curr;

	curr = tokens;
	if (ft_strcmp(curr->content, "|") == 0)
	{
		if (!prev || !curr->next)
			return (our_perror("syntax error near unexpected token `|`\n"));
		else if (ft_strcmp(curr->next->content, "|") == 0)
			return (our_perror("syntax error near unexpected token `|`\n"));
	}
	return (0);
}

static int	check_quotes_syntax(t_token *tokens)
{
	char	quote;
	char	*str;
	int		flag;
	int		i;

	flag = 0;
	i = -1;
	str = tokens->content;
	while (str[++i])
	{
		if ((str[i] == '\'' || str[i] == '"') && flag == 0)
		{
			quote = str[i++];
			flag = 1;
		}
		if (flag == 1 && str[i] == quote)
		{
			flag = 0;
			quote = '\0';
		}
	}
	if (flag == 1)
		return(our_perror("error in quoting\n"));
	else
		return (0);
}

int check_syntax_error(t_token *tokens, t_mem *gc)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (check_heredoc(tokens, gc) == 1)
			return (1);
		else if (check_append(tokens, gc) == 1)
			return (1);
		else if (check_red_out(tokens, prev, gc) == 1)
			return (1);
		else if (check_red_in(tokens, prev, gc) == 1)
			return (1);
		else if (check_pipe(tokens, prev) == 1)
			return (1);
		else if (check_quotes_syntax(tokens) == 1)
			return (1);
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}

