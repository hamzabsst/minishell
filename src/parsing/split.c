/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:51:14 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/27 15:47:30 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_token(char *str)
{
	int j = 0;
	int count = 0;
	int position = 0;
	int i = 0;
	char quote_type = '\0';

	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break;
		if (str[i] == '\"' || str[i] == '\'')
		{
			quote_type = str[i++];
			j = i;
			while (str[j])
			{
				if (str[j] == quote_type)
				{
					position = j;
					break;
				}
				j++;
			}
			while (str[i] && i != position)
				i++;
			if (str[i] == quote_type)
				i++;
			count++;
		}
		else if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
				i += 2;
			else
				i++;
			count++;
		}
		else
		{
			while (str[i] && str[i] != ' ' && str[i] != '>' && str[i] != '<' &&
				str[i] != '|' && str[i] != '\'' && str[i] != '\"')
			{
				i++;
			}
			count++;
		}
	}
	return (count);
}

static char	*get_redirection(char *str, int *start, int *last, t_mem *gc, int *flag_join)
{
	int		i;
	int		len;
	char	*word;

	i = *start;
	len = 0;
	if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			len = 2;
		else
			len = 1;
	}
	else if(str[i] == '<')
	{
		if (str[i + 1] == '<')
			len = 2;
		else
			len = 1;
	}
	*last = len;
	word = our_substr(str, *start, len, gc);
	*flag_join = 0;
	return(word);
}

static char	*get_pipe(char *str, int *start, int *last, t_mem *gc, int *flag_join)
{
	int i = 0;
	char *word;
	int len;

	i = *start;
	if (str[i] == '|')
		len = 1;
	else
		return (NULL);
	*last = len;
	word = our_substr(str, *start, len, gc);
	*flag_join = 0;
	return(word);
}

static char	*simple_word(char *str, int *start, int *last, int *flag_join, t_mem *gc)
{
	int i = 0;
	char *word;
	int len;

	i = *start;
	while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'' && str[i] != '>'
		&& str[i] != '<'  && str[i] != '|')
		i++;
	if (str[i] == '\'' || str[i] == '"')
		*flag_join = 1;
	else
		*flag_join = 0;
	*last = i - *start;
	len = i - *start;
	word = our_substr(str, *start, len, gc);
	return(word);
}

static char *get_from_squotes(char *str, int *start, int *last, int *flag_join, t_mem *gc)
{
	int i = 0;
	char *word;
	int	pos;
	int len;

	pos = *start;
	i = *start + 1;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			pos = i;
			*last = pos - *start + 1;
			break;
		}
		i++;
	}
	len = pos - *start + 1;
	word = our_substr(str, *start, len, gc);
	*flag_join = 1;
	return(word);
}

static char *get_from_dquotes(char *str, int *start, int *last, int *flag_join, t_mem *gc)
{
	int		i = 0;
	char	*word;
	int		pos;
	int		len;

	pos = *start;
	i = *start + 1;
	while (str[i])
	{
		if (str[i] == '"')
		{
			pos = i;
			*last = pos - *start + 1;
			break;
		}
		i++;
	}
	len = pos - *start + 1;
	word = our_substr(str, *start, len, gc);
	*flag_join = 1;
	return (word);
}

static int	len(char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}
char	**mysplit(char *str, t_mem *gc)
{
	int to_join = 0;
	int	last = 0;
	int i = 0;
	int	k = 0;
	char **word;

	word = ft_malloc(gc, sizeof(char *) * (count_token(str) + 1));
	if (!word)
		return (NULL);
	while (i < len(str) && str[i]) //cdcevrf ''s'fd'f'fs'"""sfa'""
	{
		while (str[i] == ' ')
		{
			i++;
			to_join = 0;
		}
		if (str[i] == '\0')
			break;
		if (str[i] == '"')
		{
			if (to_join == 1)
				word[k - 1] = our_strjoin(gc, word[k - 1], get_from_dquotes(str, &i, &last, &to_join, gc));
			else
				word[k++] = get_from_dquotes(str, &i, &last, &to_join, gc);
		}
		else if (str[i] == '\'')
		{
			if (to_join == 1)
				word[k - 1] = our_strjoin(gc, word[k - 1], get_from_squotes(str, &i, &last, &to_join, gc));
			else
				word[k++] = get_from_squotes(str, &i, &last, &to_join, gc);
		}
		else if (str[i] == '>' || str[i] == '<')
			word[k++] = get_redirection(str, &i, &last, gc, &to_join);
		else if(str[i] == '|')
			word[k++] = get_pipe(str, &i, &last, gc, &to_join);
		else if (str[i] && str[i] != '"' && str[i] != '\'' && str[i] != '>'
			&& str[i] != '<' && str[i] != '|')
		{
			if (to_join == 1)
				word[k - 1] = our_strjoin(gc, word[k - 1], simple_word(str, &i, &last, &to_join, gc));
			else
				word[k++] = simple_word(str, &i, &last, &to_join, gc);
		}
		i += last;
	}
	word[k] = NULL;
	return (word);
}
