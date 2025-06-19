/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:57:49 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/19 12:03:20 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void skip_inside_quotation(char *str, int *i)
{
	int j = 0;
	char quote_type;
	int position = 0;

	quote_type = str[*i];
	(*i)++;
	j = *i;
	while (str[j] && str[j] != ' ')
	{
		if (str[j] == quote_type)
			position = j;
		j++;
	}
	while (str[*i] && *i != position)
		(*i)++;
	if (str[*i] == quote_type)
		(*i)++;
}
static int	count_token(char *str)
{
	int count = 0;
	int i = 0;

	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break;
		count++;
		if (str[i] == '\"' || str[i] == '\'')
			skip_inside_quotation(str, &i);
		else if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
				i += 2;
			else
				i++;
		}
		else
			while (str[i] && str[i] != ' ' && str[i] != '>' && str[i] != '<' &&
				str[i] != '|' && str[i] != '\'' && str[i] != '\"')
				i++;
	}
	printf("coounnt %d", count);
	return (count);
}
char *get_from_dquotes(char *str, int *start, int *last)
{
	int i;
	char *word;
	int	pos;
	int len;

	pos = 0;
	i = *start + 1;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '"')
		{
			pos = i;
			*last = pos - *start + 1;
		}
		i++;
	}
	len = pos - *start + 1;
	word = ft_substr(str, *start, len);
	return(word);
}
char *get_from_squotes(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int	pos;
	int len;

	pos = 0;
	i = *start + 1;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '\'')
		{
			pos = i;
			*last = pos - *start;
		}
		i++;
	}
	len = pos - *start;
	word = ft_substr(str, *start, len);
	return(word);
}

char	*simple_word(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int len;

	i = *start;
	while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'' && str[i] != '>'
		&& str[i] != '<'  && str[i] != '|')
		i++;
	*last = i - *start;
	len = i - *start;
	word = ft_substr(str, *start, len);
	return(word);
}
char	*get_redirection(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int len = 0;

	i = *start;
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
	*last = len + 1;
	word = ft_substr(str, *start, len);
	return(word);
}

char	*get_pipe(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int len;

	i = *start;
	if (str[i] == '|')
		len = 1;
	else
		return (NULL);
	*last = len + 1;
	word = ft_substr(str, *start, (len));
	return(word);
}
char **mysplit(t_cmd *cmd, char *str)
{
	int	last = 0;
	int i = 0;
	int	k = 0;
	char **word = ft_malloc(cmd->collector, sizeof(char *) * count_token(str) + 1);
	if (!word)
		return (NULL);
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] == '"')
			word[k++] = get_from_dquotes(str, &i, &last);
		else if (str[i] == '\'')
			word[k++] = get_from_squotes(str, &i, &last);
		else if (str[i] == '>' || str[i] == '<')
			word[k++] = get_redirection(str, &i, &last);
		else if(str[i] == '|')
			word[k++] = get_pipe(str, &i, &last);
		else if (str[i] && str[i] != '"' && str[i] != '\'' && str[i] != '>'
			&& str[i] != '<'  && str[i] != '|')
			word[k++] = simple_word(str, &i, &last);
		i += last;
	}
	word[k] = NULL;
	return (word);
}
