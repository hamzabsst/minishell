/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:44:43 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/07 15:23:37 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes_syntax(t_token *tokens)
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
		if ((str[i] == '\'' || str[i] == '"') && !flag)
		{
			quote = str[i];
			flag = 1;
		}
		else if (flag && str[i] == quote)
		{
			flag = 0;
			quote = '\0';
		}
	}
	if (flag)
		return (1);
	else
		return (0);
}

static char	*get_from_squotes(t_split *s)
{
	int	start;
	int	i;

	start = s->i;
	i = s->i + 1;
	while (s->line[i] && s->line[i] != '\'')
		i++;
	if (s->line[i] == '\'')
		i++;
	s->last = i - start;
	s->to_join = 1;
	return (our_substr(s->line, start, s->last, s->gc));
}

static char	*get_from_dquotes(t_split *s)
{
	int	start;
	int	i;

	start = s->i;
	i = s->i + 1;
	while (s->line[i] && s->line[i] != '\"')
		i++;
	if (s->line[i] == '\"')
		i++;
	s->last = i - start;
	s->to_join = 1;
	return (our_substr(s->line, start, s->last, s->gc));
}

void	handle_quotes(t_split *s)
{
	if (s->line[s->i] == '\'')
	{
		if (s->to_join)
			s->word[s->j - 1] = our_strjoin(s->gc, s->word[s->j - 1],
					get_from_squotes(s));
		else
			s->word[s->j++] = get_from_squotes(s);
	}
	else if (s->line[s->i] == '\"')
	{
		if (s->to_join)
			s->word[s->j - 1] = our_strjoin(s->gc,
					s->word[s->j - 1], get_from_dquotes(s));
		else
			s->word[s->j++] = get_from_dquotes(s);
	}
}
