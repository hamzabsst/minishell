/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:10:58 by abchaman          #+#    #+#             */
/*   Updated: 2025/07/02 23:20:18 by hbousset         ###   ########.fr       */
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

static void	repl_dollar(t_token **tokens, t_token *curr, int e_code, t_mem *gc)
{
	t_token	*new_list;
	t_token	*end;
	char	*code;

	new_list = NULL;
	code = ft_itoa(e_code);
	ft_add_ptr(gc, code);
	add_token(&new_list, code, "WORD", gc);
	end = new_list;
	while (end && end->next)
		end = end->next;
	repl_token(tokens, curr, new_list, end);
}

void	get_exit(t_token **tokens, int exit_code, t_mem *gc)
{
	t_token	*curr;
	t_token	*next;

	curr = *tokens;
	while (curr)
	{
		next = curr->next;
		if ((!ft_strcmp(curr->content, "$?")
				|| !ft_strcmp(curr->content, "\"$?\""))
			&& !ft_strcmp(curr->type, "WORD"))
		{
			repl_dollar(tokens, curr, exit_code, gc);
		}
		curr = next;
	}
}
