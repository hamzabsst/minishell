/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:33:24 by abchaman          #+#    #+#             */
/*   Updated: 2025/05/28 10:43:44 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strndup(t_cmd *cmd, char *str, size_t len, char skip)
{
	size_t	i;
	char	*results;
	int k;

	k = 0;
	results = ft_malloc(cmd->collector, len + 1);
	if (!results)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if(str[i] != skip)
			results[k++] = str[i];
		i++;
	}
	results[k] = '\0';
	return (results);
}
static int count_tokens(char *str)
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
		count++;
		if (str[i] == '\"' || str[i] == '\'')
		{
			quote_type = str[i++];
			while (str[j])
			{
				if (str[j] == quote_type)
					position = j;
				j++;
			}
			while (str[i] && i != position)
				i++;
			if (str[i] == quote_type)
				i++;
		}
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
	// printf("%s\n", str);
	// printf("%d\n", count);
	return (count);
}

char	**smart_split(t_cmd *cmd, char *str)
{
	int j;
	int i;
	int k;
	int start;
	int in_quote;
	char quote_type;
	char **tokens;
	int token_count;
	int position;

	position = 0;
	j = 0;
	k = 0;
	i = 0;
	start = 0;
	in_quote = 0;
	quote_type = '\0';
	token_count = count_tokens(str);

	tokens = ft_malloc(cmd->collector , sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	while (str[i] == ' ')
		i++;
	if(str[i] == '|')
	{
		printf("syntax error near unexpected token `%c'\n", str[i]);
	}
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break;
		start = i;
		if (str[i] == '\"' || str[i] == '\'')
		{
			tokens[k++] = insidequotes(cmd, str, &i);
		}
		else if (str[i] == '>')
		{
			if (in_quote == 0)
			{
				int len = 1;
				if (str[i + 1] == str[i])
					len = 2;
				tokens[k++] = ft_strndup(cmd, &str[i], len, 0);
				i += len;
			}
		}
		else if (str[i] == '<')
		{
			if (in_quote == 0)
			{
				int len = 1;
				if (str[i + 1] == str[i])
					len = 2;
				tokens[k++] = ft_strndup(cmd, &str[i], len, 0);
				i += len;
			}
		}
		else if (str[i] == '|')
		{
			if (str[i + 1] != str[i])
			{
				tokens[k++] = ft_strndup(cmd, &str[i], 1, 0);
			}
			i++;
		}
		else
		{
			while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '\"' &&
				str[i] != '>' && str[i] != '<' && str[i] != '|')
				i++;
			if (i > start)
				tokens[k++] = ft_strndup(cmd, &str[start], i - start, 0);
		}
	}
	tokens[k] = NULL;
	return (tokens);
}
