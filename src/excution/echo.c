/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:23:03 by hbousset          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/19 16:08:04 by abchaman         ###   ########.fr       */
=======
/*   Updated: 2025/04/19 21:03:41 by hbousset         ###   ########.fr       */
>>>>>>> 5784d933ed07120410296bbaa5c786ba2528cb8e
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_echo(char **argv)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	if (argv[i] && !ft_strcmp(argv[i], "-n"))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
