/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:57:03 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/19 17:52:20 by hbousset         ###   ########.fr       */
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

static void	print_arg_filtered(const char *arg)
{
	while (*arg)
	{
		if (*arg != '"' && *arg != '\'')
			printf("%c", *arg);
		arg++;
	}
}

int	builtin_echo(char **av)
{
	int		i;
	int		newline;

	newline = 1;
	i = 1;
	handle_flag(av, &i, &newline);
	while (av[i])
	{
		print_arg_filtered(av[i]);
		if (av[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
