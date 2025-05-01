/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:51:12 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/01 14:30:58 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **argv, char ***env)
{
	int	exit_code;

	write(1, "exit\n", 5);
	if (!argv[1])
	{
		ft_free(*env);
		exit(0);
	}
	if (!is_numeric(argv[1]))
	{
		ft_perror("exit: ");
		ft_perror(argv[1]);
		ft_perror(": numeric argument required\n");
		ft_free(*env);
		exit(255);
	}
	if (argv[2])
		return (ft_perror("exit: too many arguments\n"));
	exit_code = ft_atoi(argv[1]);
	ft_free(*env);
	exit(exit_code);
}
