/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:10:58 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/25 21:16:53 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_token **start, t_token **end, char *content, const char *type, t_mem *gc)
{
	t_token	*new;

	new = ft_malloc(gc, sizeof(t_token));
	if (!new)
		return ;
	new->content = content;
	new->type = our_strdup(gc, type);
	new->next = NULL;
	if (!*start)
		*start = new;
	else
		(*end)->next = new;
	*end = new;
}

void	replace_token(t_token **tokens, t_token *curr, t_token *new_list, t_token *end)
{
	t_token	*tmp;

	tmp = *tokens;
	if (*tokens == curr)
		*tokens = new_list;
	else
	{
		while (tmp && tmp->next != curr)
			tmp = tmp->next;
		if (tmp)
			tmp->next = new_list;
	}
	if (end)
		end->next = curr->next;
}

static void show_exit_code(t_token **tokens, t_token *curr, int exit_code, t_mem *gc)
{
	t_token	*new_list;
	t_token	*end;
	char	*code;

	new_list = NULL;
	end = NULL;
	code = ft_itoa(exit_code);
	if (ft_strcmp(curr->content, "$?") == 0 || ft_strcmp(curr->content, "\"$?\"") == 0)
		add_token(&new_list, &end, code, "WORD", gc);
	else
		add_token(&new_list, &end, ft_strdup(curr->content), curr->type, gc);
	replace_token(tokens, curr, new_list, end);
	// free(code);
}

void get_exit(t_token **tokens, int exit_code, t_mem *gc)
{
	t_token	*curr;
	t_token	*next;

	curr = *tokens;
	while (curr)
	{
		next = curr->next;
		if ((ft_strcmp(curr->content, "$?") == 0 || ft_strcmp(curr->content, "\"$?\"") == 0)
			&& ft_strcmp(curr->type, "WORD") == 0)
		{
			show_exit_code(tokens, curr, exit_code, gc);
		}
		curr = next;
	}
}

