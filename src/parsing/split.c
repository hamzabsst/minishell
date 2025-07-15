/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:51:14 by abchaman          #+#    #+#             */
/*   Updated: 2025/07/15 15:09:26 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_redirection(t_split *s)
{
	int	len;

	len = 0;
	if (s->line[s->i] == '>')
	{
		if (s->line[s->i + 1] == '>')
			len = 2;
		else
			len = 1;
	}
	else if (s->line[s->i] == '<')
	{
		if (s->line[s->i + 1] == '<')
			len = 2;
		else
			len = 1;
	}
	s->last = len;
	s->to_join = 0;
	return (our_substr(s->line, s->i, len, s->gc));
}

static char	*simple_word(t_split *s)
{
	int	start;
	int	i;

	start = s->i;
	i = s->i;
	while (s->line[i] && s->line[i] != ' ' && s->line[i] != '\t'
		&& s->line[i] != '\n' && s->line[i] != '\"' && s->line[i] != '\''
		&& s->line[i] != '>' && s->line[i] != '<' && s->line[i] != '|')
		i++;
	s->to_join = (s->line[i] == '\'' || s->line[i] == '\"');
	s->last = i - start;
	return (our_substr(s->line, start, s->last, s->gc));
}

static void	handle_special_chars(t_split *s)
{
	if (s->line[s->i] == '>' || s->line[s->i] == '<')
		s->word[s->j++] = get_redirection(s);
	else if (s->line[s->i] == '|')
	{
		s->last = 1;
		s->to_join = 0;
		s->word[s->j++] = our_substr(s->line, s->i, 1, s->gc);
	}
	else
	{
		if (s->to_join)
			s->word[s->j - 1] = our_strjoin(s->gc,
					s->word[s->j - 1], simple_word(s));
		else
			s->word[s->j++] = simple_word(s);
	}
}

static t_split	*init_split(t_parse *data)
{
	t_split	*split;

	split = ft_malloc(data->gc, sizeof(t_split));
	if (!split)
		return (NULL);
	split->line = data->line;
	split->gc = data->gc;
	split->to_join = 0;
	split->last = 0;
	split->i = 0;
	split->j = 0;
	split->len = ft_strlen(split->line);
	split->word = ft_malloc(split->gc,
			sizeof(char *) * (count_token(split->line) + 1));
	if (!split->word)
		return (NULL);
	return (split);
}

char	**mysplit(t_parse *data)
{
	t_split	*split;

	split = init_split(data);
	if (!split || !split->word)
		return (NULL);
	while (split->i < split->len && split->line[split->i])
	{
		while (split->line[split->i] == ' '
			|| split->line[split->i] == '\t'
			|| split->line[split->i] == '\n')
		{
			split->i++;
			split->to_join = 0;
		}
		if (!split->line[split->i])
			break ;
		if (split->line[split->i] == '\'' || split->line[split->i] == '\"')
			handle_quotes(split);
		else
			handle_special_chars(split);
		split->i += split->last;
	}
	split->word[split->j] = NULL;
	return (split->word);
}
