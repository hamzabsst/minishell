/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:30:00 by abchaman          #+#    #+#             */
/*   Updated: 2025/07/03 00:17:12 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_quoted_chars(char *input, int *i)
{
	char	quote;
	int		count;

	count = 0;
	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
	{
		(*i)++;
		count++;
	}
	return (count);
}

static int	count_without_quotes(t_token *curr)
{
	char	*input;
	int		count;
	int		i;

	input = curr->content;
	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			count += count_quoted_chars(input, &i);
		else
			count++;
		i++;
	}
	return (count);
}

static char	*process_quotes(char *input, int *i, char *word, int *j)
{
	char	quote;

	if (input[*i] == '\'' || input[*i] == '\"')
	{
		quote = input[(*i)++];
		while (input[*i] && input[*i] != quote)
			word[(*j)++] = input[(*i)++];
	}
	else
		word[(*j)++] = input[*i];
	(*i)++;
	return (word);
}

static void	remove_quotation(t_token **tokens, t_token *curr, t_mem *gc)
{
	t_token	*new_list;
	char	*word;
	int		i;
	int		j;

	new_list = NULL;
	i = 0;
	j = 0;
	word = ft_malloc(gc, count_without_quotes(curr) + 1);
	while (curr->content[i])
		process_quotes(curr->content, &i, word, &j);
	word[j] = '\0';
	add_token(&new_list, word, "WORD", gc);
	while (new_list && new_list->next)
		new_list = new_list->next;
	repl_token(tokens, curr, new_list, new_list);
}

void	check_quotes(t_token **tokens, t_mem *gc)
{
	t_token	*curr;
	t_token	*next;

	curr = *tokens;
	while (curr)
	{
		next = curr->next;
		if (!ft_strcmp(curr->type, "WORD")
			&& (ft_strchr(curr->content, '\'')
				|| ft_strchr(curr->content, '\"')))
		{
			remove_quotation(tokens, curr, gc);
		}
		curr = next;
	}
}
