/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:57:03 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/25 11:33:30 by hbousset         ###   ########.fr       */
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
			i++;
			while (line[i] != quote_type && line[i])
				i++;
			if(quote_type != line[i])
				return (ft_perror("syntax error in quoting\n"), -1);
		}
		i++;
	}
	return (0);
}

