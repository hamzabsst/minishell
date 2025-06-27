/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:10:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/25 20:49:59 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_cmd *cmd)
{
	t_env	*current;
	t_env	*prev;
	int		i;

	i = 1;
	if (!cmd->av[1])
		return (0);
	while (cmd->av[i])
	{
		if (cmd->av[i][0] == '-')
			return (our_perror("unset: options are not allowed\n"));
		current = cmd->env;
		prev = NULL;
		while (current)
		{
			if (current->var && ft_strcmp(current->var, cmd->av[i]) == 0)
			{
				if (prev)
					prev->next = current->next;
				else
					cmd->env = current->next;
				break;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
	return (0);
}
