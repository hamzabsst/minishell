/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:57:03 by abchaman          #+#    #+#             */
/*   Updated: 2025/05/30 09:43:48 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_quotes_error(char *line)
{
	int		i = 0;
	char	quote_type;
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\"')
		{
			quote_type = line[i];
			while (line[i] != '\"' && line[i] != '\'')
			{
				i++;
			}
			if ()
			{
				/* code */
			}

		}
		i++;
	}

}

int main()
{
	char str[] = "";
	int s = handle_quotes_error(str);
	printf("%d", s);
}
