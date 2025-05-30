/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:57:03 by abchaman          #+#    #+#             */
/*   Updated: 2025/05/30 10:20:15 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_quotes_error(char *line)
{
	int		i = 0;
	char	quote_type;
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			quote_type = line[i];
			i += 1;
			while (line[i] != quote_type && line[i])
				i++;
			if(quote_type != line[i])
			{
				printf("syntax error in quoting\n");
				return(-1);
			}
		}
		i++;
	}
	return (0);
}

