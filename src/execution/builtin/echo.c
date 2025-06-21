/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:57:03 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 11:37:42 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_flag(char **av, int *i, int *newline)
{
	int		valid;
	char	*ptr;

	while (av[*i] && !ft_strncmp(av[*i], "-n", 2))
	{
		ptr = av[*i] + 1;
		valid = 1;
		while (*ptr)
		{
			if (*ptr != 'n')
			{
				valid = 0;
				break ;
			}
			ptr++;
		}
		if (!valid)
			break ;
		*newline = 0;
		(*i)++;
	}
}

int	builtin_echo(t_cmd *cmd)
{
	int		i;
	int		newline;

	newline = 1;
	i = 1;
	handle_flag(cmd->av, &i, &newline);
	while (cmd->av[i])
	{
		printf("%s", cmd->av[i]);
		if (cmd->av[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
