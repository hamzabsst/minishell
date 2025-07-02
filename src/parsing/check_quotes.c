/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:30:00 by abchaman          #+#    #+#             */
/*   Updated: 2025/07/02 02:53:07 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_without_quotes(t_token *curr)
{
	int		count;
	char	quote;
	char	*input;
	int		i;

	input = curr->content;
	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
			{
				i++;
				count++;
			}
		}
		else if (input[i] == '"')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
			{
				i++;
				count++;
			}
		}
		else
			count++;
		i++;
	}
	return (count);
}

static void	remove_quotation(t_token **tokens, t_token *curr, t_mem *gc)
{
	char	*input;
	char	*word;
	int		i;
	int		k;
	t_token	*new_list;
	t_token	*end;

	new_list = NULL;
	end = NULL;
	k = 0;
	i = 0;
	input = curr->content;
	word = ft_malloc(gc, count_without_quotes(curr) + 1);
	while (input[i])
	{
		if (input[i] == '\'')
		{
			i++;
			while (input[i] && input[i] != '\'')
				word[k++] = input[i++];
		}
		else if (input[i] == '\"')
		{
			i++;
			while (input[i] && input[i] != '\"')
				word[k++] = input[i++];
		}
		else
			word[k++] = input[i];
		i++;
	}
	word[k] = '\0';
	add_token(&new_list, &end, word, "WORD", gc);
	replace_token(tokens, curr, new_list, end);
}

void	check_quotes(t_token **tokens, t_mem *gc)
{
	t_token	*curr;
	t_token	*next;

	curr = *tokens;
	while (curr)
	{
		next = curr->next;
		if (ft_strcmp(curr->type, "WORD") == 0 && (ft_strchr(curr->content, '\'') || ft_strchr(curr->content, '\"')))
		{
			remove_quotation(tokens, curr, gc);
		}
		curr = next;
	}
}
