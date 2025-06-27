/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:51:12 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/27 18:07:48 by hbousset         ###   ########.fr       */
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
	unsigned long long	exit_code_ll;
	int					exit_code;

	if (!cmd->forked)
		ft_putstr_fd("exit\n", 2);
	if (!cmd->av[1])
		(ft_free_all(cmd->gc), exit(0));
	if (!is_num(cmd->av[1]))
	{
		ft_perror("exit: ", cmd->av[1],
			": numeric argument required\n", cmd->gc);
		(ft_free_all(cmd->gc), exit(2));
	}
	exit_code_ll = ft_atoll(cmd->av[1]);
	if (exit_code_ll == LLONG_MAX + 1ULL)
	{
		ft_perror("exit: ", cmd->av[1],
			": numeric argument required\n", cmd->gc);
		(ft_free_all(cmd->gc), exit(2));
	}
	if (cmd->av[2])
		return (our_perror("exit: too many arguments\n"));
	exit_code = (int)(exit_code_ll & 0xFF);
	ft_free_all(cmd->gc);
	exit(exit_code);
}
