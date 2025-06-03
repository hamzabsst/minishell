/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:10:58 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/04 00:41:13 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	quotes_inside_quotes(char *line, int *last, int *start, char quote_type)
{
	int	len;
	int	j;

	j = *start;
	while (line[j])
	{
		while (line[j] != quote_type)
		{
			j++;
		}
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
			*i += 1;
			start = *i;
			k = *i;
			j = *i;
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
					len = quotes_inside_quotes(line, &last, &start, quote_type); // error ya morahiq value stored in this var is never read
					break;
				}
				k++;
			}
			len = last - start;
			if (extra != NULL)
			{
				first = ft_strndup_mem(cmd, &line[*i], len, quote_type, 0);
				extra = ft_strjoin_mem(cmd->collector, extra, first);
				//free(first); //adak morahiq baraka mn free HHHHH
			}
			else if (second != NULL)
			{
				first = ft_strndup_mem(cmd, &line[*i], len, quote_type, 0);
				extra = ft_strjoin_mem(cmd->collector, second, first);
				//free(first);
			}
			else if(first != NULL)
			{
				second = ft_strndup_mem(cmd, &line[*i], len, quote_type, 0);
				extra = ft_strjoin_mem(cmd->collector, first, second);
				//free(second);
			}
			else
				first = ft_strndup_mem(cmd, &line[*i], len, quote_type, 0);
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
				second = ft_strndup_mem(cmd, &line[start], len, 0, 0);
				extra = ft_strjoin_mem(cmd->collector, extra, second);
				//free(second);
			}
			else if (first != NULL)
			{
				second = ft_strndup_mem(cmd, &line[start], len, 0, 0);
				extra = ft_strjoin_mem(cmd->collector, first, second);
				//free(second);
			}
			else if (second != NULL)
			{
				first = ft_strndup_mem(cmd, &line[start], len, 0, 0);
				extra = ft_strjoin_mem(cmd->collector, second, first);
				//free(first);
			}
			else
				second = ft_strndup_mem(cmd, &line[start], len, 0, 0);
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
