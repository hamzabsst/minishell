/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:51:14 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/23 10:43:57 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(char *str)
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
	//printf("%d\n", count);
	return (count);
}

//!➜  ~/minishell echo ""''sdldkl''""lldlds dlsldldl""
//!==213010== Invalid write of size 4
char	**smart_split(t_cmd *cmd, char *str)
{
	int i;
	int j;
	int start;
	char single_quote_type;
	char double_quote_type;
	char **tokens;
	int token_count;
	int is_space;

	j = 0;
	i = 0;
	single_quote_type = '\0';
	double_quote_type = '\0';
	token_count = count_tokens(str);

	tokens = ft_malloc(cmd->gc , sizeof(char *) * (token_count * 2 + 10)); //allocated more size
	if (!tokens)
		return (NULL);
	cmd->quote_flags = ft_malloc(cmd->gc, sizeof(int) * (token_count + 1));
	if (!cmd->quote_flags)
		return (NULL);
	while (str[i] == ' ')
		i++;
	while (str[i])
	{
		is_space = 0;
		while (str[i] == ' ') //echo df, between echo and dd has a space, so the flag will be 1;
		{
			i++;
			is_space = 1;
		}
		if (!str[i])
			break;
		if (str[i] == '\'')
		{
			cmd->quote_flags[j] = 1;
			if (j > 0 && (cmd->quote_flags[j - 1] == 1 || cmd->quote_flags[j - 1] == 2) && is_space == 0)
			{
				char *temp = our_strjoin(cmd->gc, tokens[j - 1] ,insidequotes(cmd, str, &i));
				tokens[j - 1] = temp;
			}
			else
				tokens[j++] = insidequotes(cmd, str, &i);
		}
		else if (str[i] == '\"')
		{
			cmd->quote_flags[j] = 2;
			if (j > 0 && (cmd->quote_flags[j - 1] == 1 || cmd->quote_flags[j - 1] == 2) && is_space == 0)
			{
				char *temp = our_strjoin(cmd->gc, tokens[j - 1] ,insidequotes(cmd, str, &i));
				tokens[j - 1] = temp;
			}
			else
				tokens[j++] = insidequotes(cmd, str, &i);
		}
		// else if (str[i] == '$')
		// {
		// 	if (j > 0 && (cmd->quote_flags[j - 1] == 1 || cmd->quote_flags[j - 1] == 2))
		// 	{
		// 		char *temp = our_strjoin(cmd->gc, tokens[j - 1], get_from_env(cmd, env ,&str[i]));
		// 		tokens[j - 1] = temp;
		// 	}
		// 	else
		// 		tokens[j++] = get_from_env(cmd, env ,&str[i]);
		// }
		else if (str[i] == '>' || str[i] == '<')
		{
			cmd->quote_flags[j] = 0;
			int len = 1;
			if (str[i + 1] == str[i])
				len = 2;
			tokens[j++] = our_strndup(cmd->gc, &str[i], len, 0, 0);
			i += len;
		}
		else if (str[i] == '|')
		{
			cmd->quote_flags[j] = 0;
			if (str[i + 1] != str[i])
				tokens[j++] = our_strndup(cmd->gc, &str[i], 1, 0, 0);
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
					break;
				}
				else if (str[i] == '\"')
				{
					double_quote_type = str[i++];
					while (str[i] && str[i] != double_quote_type)
						i++;
					if (str[i] == double_quote_type)
						i++;
					break;
				}
				else if (str[i] == '>' || str[i] == '<')
					break;
				else
					i++;
			}
			if (i > start)
			{
				if(double_quote_type != '\0')
					cmd->quote_flags[j] = 2;
				else if(single_quote_type != '\0')
					cmd->quote_flags[j] = 1;
				else
					cmd->quote_flags[j] = 0;
				if (j > 0 && (cmd->quote_flags[j - 1] == 1 || cmd->quote_flags[j - 1] == 2) && is_space == 0)
				{
					char *temp = our_strjoin(cmd->gc, tokens[j - 1] , our_strndup(cmd->gc, &str[start], i - start, 0 ,0));
					tokens[j - 1] = temp;
				}
				else
					tokens[j++] = our_strndup(cmd->gc, &str[start], i - start, 0 , 0);
			}
		}
	}
	tokens[j] = NULL;
	if (j < token_count * 2 + 10)
		cmd->quote_flags[j] = 0;
	return (tokens);
}
