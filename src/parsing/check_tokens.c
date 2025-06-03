/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:06:02 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/03 21:09:02 by hbousset         ###   ########.fr       */
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
int	check_heredoc(char *line)
{
	int		i;
	char	quote_type;

	i = 0;
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
