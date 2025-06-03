/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:51:14 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/03 10:44:09 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strndup(t_cmd *cmd, char *str, size_t len, char skip_single_q, char skip_double_q)
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
		if(str[i] != skip_single_q && str[i] != skip_double_q)
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
	char single_quote_type;
	char double_quote_type;
	char **tokens;
	int token_count;
	int token_inside_quote;
	int is_space;

	token_inside_quote = 0;
	j = 0;
	k = 0;
	i = 0;
	start = 0;
	in_quote = 0;
	single_quote_type = '\0';
	double_quote_type = '\0';
	token_count = count_tokens(str);

	tokens = ft_malloc(cmd->collector , sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	cmd->quote_flags = ft_malloc(cmd->collector, sizeof(int) * (token_count + 1));
	if (!cmd->quote_flags)
		return (NULL);
	while (str[i] == ' ')
		i++;
	while (str[i])
	{
		is_space = 0;
		while (str[i] == ' ') //echo df, between echo and dd has a space, so the flag will be 0;
		{
			i++;
			is_space = 1;
		}
		if (!str[i])
			break;
		start = i;
		if (str[i] == '\"' || str[i] == '\'')
		{
			cmd->quote_flags[k] = 1;
			if (k > 0 && cmd->quote_flags[k - 1] == 1 && is_space == 0)
			{
				char *temp = ft_strjoin(tokens[k - 1] ,insidequotes(cmd, str, &i));
				free(tokens[k - 1]);
				tokens[k - 1] = temp;
			}
			else
				tokens[k++] = insidequotes(cmd, str, &i);
		}
		else if (str[i] == '>' || str[i] == '<')
		{
			cmd->quote_flags[k] = 0;
			int len = 1;
			if (str[i + 1] == str[i])
				len = 2;
			tokens[k++] = ft_strndup(cmd, &str[i], len, 0, 0);
			i += len;
		}
		else if (str[i] == '|')
		{
			cmd->quote_flags[k] = 0;
			if (str[i + 1] != str[i])
				tokens[k++] = ft_strndup(cmd, &str[i], 1, 0, 0);
			i++;
		}
		else
		{
			start = i;
			while (str[i] && str[i] != ' ' && str[i] != '|')
			{
				if (str[i] == '\'')
				{
					single_quote_type = str[i++];
					while (str[i] && str[i] != single_quote_type)
						i++;
					if (str[i] == single_quote_type)
						i++;
				}
				else if (str[i] == '\"')
				{
					double_quote_type = str[i++];
					while (str[i] && str[i] != double_quote_type)
						i++;
					if (str[i] == double_quote_type)
						i++;
				}

				else
					i++;
			}
			if (i > start)
			{
				cmd->quote_flags[k] = 0;
				if (k > 0 && cmd->quote_flags[k - 1] == 1 && is_space == 0)
				{
					char *temp = ft_strjoin(tokens[k - 1] , ft_strndup(cmd, &str[start], i - start, single_quote_type, double_quote_type));
					free(tokens[k - 1]);
					tokens[k - 1] = temp;
				}
				else
					tokens[k++] = ft_strndup(cmd, &str[start], i - start, single_quote_type, double_quote_type);
			}
		}
	}
	tokens[k] = NULL;
	cmd->quote_flags[k] = 0;
	return (tokens);
}
