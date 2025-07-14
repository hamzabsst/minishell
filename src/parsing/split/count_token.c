/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:28:52 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/02 23:41:28 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quote(char *s, int *i)
{
	int		j;
	char	q;

	q = s[(*i)++];
	j = *i;
	while (s[j] && s[j] != q)
		j++;
	while (s[*i] && *i != j)
		(*i)++;
	if (s[*i] == q)
		(*i)++;
	return (1);
}

static int	handle_operator(char *s, int *i)
{
	if ((s[*i] == '>' && s[*i + 1] == '>')
		|| (s[*i] == '<' && s[*i + 1] == '<'))
		*i += 2;
	else
		(*i)++;
	return (1);
}

static int	handle_word(char *s, int *i)
{
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && s[*i] != '\n' 
		&& s[*i] != '>' && s[*i] != '<' && s[*i] != '|' 
		&& s[*i] != '\'' && s[*i] != '\"')
		(*i)++;
	return (1);
}

int	count_token(char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
			i++;
		if (!s[i])
			break ;
		if (s[i] == '\'' || s[i] == '\"')
			count += handle_quote(s, &i);
		else if (s[i] == '>' || s[i] == '<' || s[i] == '|')
			count += handle_operator(s, &i);
		else
			count += handle_word(s, &i);
	}
	return (count);
}
