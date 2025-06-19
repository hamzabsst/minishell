/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:10:58 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/17 14:32:43 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	quotes_inside_quotes(char *line, int *last, int *start, char quote_type)
{
	int	len;
	int	j;

	j = *start + 1;
	while (line[j])
	{
		while (line[j] != quote_type)
			j++;
		if (line[j] == quote_type)
		{
			*last = j;
			break;
		}
		j++;
	}
	len = *last - *start;
	return (len);
}

char	*insidequotes(t_cmd *cmd, char *line, int *i)
{
	char *first = NULL;
	char *second = NULL;
	char *extra = NULL;
	int len = 0;
	int start = 0;
	int last = 0;
	char quote_type;
	char quote_stop;
	int j = 0;
	int k = 0;
	while (line[*i])
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			quote_type = line[*i];
			if (quote_type == '\'')
				quote_stop = '\"';
			else
				quote_stop = '\'';
			start = *i;
			*i += 1;
			k = *i + 1;
			j = *i + 1;
			while (line[j])
			{
				if(line[j] == quote_type)
				{
					last = j;
					break;
				}
				j++;
			}
			while (k < last)
			{
				if (line[k] == quote_stop)
				{
					quotes_inside_quotes(line, &last, &start, quote_type);
					break;
				}
				k++;
			}
			len = last - start + 1;
			if (extra != NULL)
			{
				first = our_strndup(cmd->collector , &line[start], len);
				extra = our_strjoin(cmd->collector, extra, first);
			}
			else if (second != NULL)
			{
				first = our_strndup(cmd->collector , &line[start], len);
				extra = our_strjoin(cmd->collector, second, first);
			}
			else if(first != NULL)
			{
				second = our_strndup(cmd->collector , &line[start], len);
				extra = our_strjoin(cmd->collector, first, second);
			}
			else
				first = our_strndup(cmd->collector , &line[start], len);
			*i = last + 1;
			break;
		}
		else if (line[*i] != ' ' && line[*i] != '\'' && line[*i] != '"')
		{
			start = *i;
			while (line[*i] != ' ' && line[*i] && line[*i] != '\'' && line[*i] != '"')
				(*i)++;
			(*i)--;
			last = *i;
			len = last - start + 1;
			if (extra != NULL)
			{
				second = our_strndup(cmd->collector , &line[start], len);
				extra = our_strjoin(cmd->collector, extra, second);
			}
			else if (first != NULL)
			{
				second = our_strndup(cmd->collector , &line[start], len);
				extra = our_strjoin(cmd->collector, first, second);
			}
			else if (second != NULL)
			{
				first = our_strndup(cmd->collector , &line[start], len);
				extra = our_strjoin(cmd->collector, second, first);
			}
			else
				second = our_strndup(cmd->collector , &line[start], len);
		}
		else if (line[*i] == ' ' || line[*i] == '>' || line[*i] == '<' || line[*i] == '|')
			break;
		(*i)++;
	}
	if (extra != NULL)
		return (extra);
	else if (first != NULL)
		return (first);
	else if(second != NULL)
		return (second);
	else
		return (NULL);
}
