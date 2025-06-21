/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:51:12 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 11:54:52 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_num(const char *str)
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

int	builtin_exit(t_cmd *cmd)
{
	int	exit_code;

	write(1, "exit\n", 5);
	if (!cmd->av[1])
	{
		ft_free_ptr(cmd->gc, cmd->env);
		exit(0);
	}
	if (!is_num(cmd->av[1]))
	{
		ft_perror("exit: ");
		ft_perror(cmd->av[1]);
		ft_perror(": numeric argument required\n");
		ft_free_ptr(cmd->gc, cmd->env);
		exit(255);
	}
	if (cmd->av[2])
		return (ft_perror("exit: too many arguments\n"));
	exit_code = ft_atoi(cmd->av[1]);
	ft_free_ptr(cmd->gc, cmd->env);
	exit(exit_code);
}
