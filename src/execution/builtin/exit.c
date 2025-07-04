/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:51:12 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/04 14:33:11 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_not_num(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	cleanup_and_exit(t_cmd *cmd)
{
	int	exit_code;

	exit_code = cmd->exit_code;
	restore_io(cmd->in_copy, cmd->out_copy);
	ft_free_all(cmd->gc);
	exit(exit_code);
}

int	builtin_exit(t_cmd *cmd)
{
	unsigned long long	exit_code_ll;

	if (!cmd->forked)
		ft_putstr_fd("exit\n", 2);
	if (!cmd->av[1])
		cleanup_and_exit(cmd);
	exit_code_ll = ft_atoll(cmd->av[1]);
	if (is_not_num(cmd->av[1]) || exit_code_ll == LLONG_MAX + 1ULL)
	{
		ft_error("exit: ", cmd->av[1],
			": numeric argument required\n", cmd->gc);
		cmd->exit_code = 2;
		cleanup_and_exit(cmd);
	}
	if (cmd->av[2])
		return (our_perror("exit: too many arguments\n"));
	cmd->exit_code = (int)(exit_code_ll & 0xFF);
	cleanup_and_exit(cmd);
	return (0);
}
