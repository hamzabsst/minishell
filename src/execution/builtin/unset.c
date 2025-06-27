/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:10:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/27 18:09:37 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_variable(t_cmd *cmd, char *var_name)
{
	t_env	*current;
	t_env	*prev;

	current = cmd->env;
	prev = NULL;
	while (current)
	{
		if (current->var && ft_strcmp(current->var, var_name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				cmd->env = current->next;
			break ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!cmd->av[1])
		return (0);
	while (cmd->av[i])
	{
		if (cmd->av[i][0] == '-')
			return (our_perror("unset: options are not allowed\n"));
		unset_variable(cmd, cmd->av[i]);
		i++;
	}
	return (0);
}
