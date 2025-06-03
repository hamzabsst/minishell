/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:06:02 by abchaman          #+#    #+#             */
/*   Updated: 2025/05/31 10:25:34 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int check_append(char *line)
// {

// }
// int check_red_in(char *line, int *i)
// {
// 	int	start;

// 	start = *i;
// 	while (line[*i])
// 	{

// 	}
// }
// int check_red_out(char *line)
// {

// }
int check_heredoc(char *line)
{
	int	i = 0;
	char quote_type;
	while (line[i])
	{
		if(line[i] == '\"' || line[i] == '\'')
		{
			quote_type = line[i];
			i++;
			if(line[i] == '<' && line[i + 1] == '<')
				i += 2;
			if(line[i] == quote_type)
				return(1);
		}
		i++;
	}
	return (0);
}
