/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:57:03 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/03 21:20:19 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes_error(char *line)
{
	int		i;
	char	quote_type;

	i = 0;
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
				ft_perror("syntax error in quoting\n");
				//always use perror on errors
				//why ?
				//printf is part of the standard I/O library,
				//which introduces buffering and is not allowed in strict shells like minishell
				//stupid AAHH
				return(-1); //chkun ki returni -1 HHHHHHH
			}
		}
		i++;
	}
	return (0);
}

