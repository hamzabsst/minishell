/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:06:02 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/16 17:47:45 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
