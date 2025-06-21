/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:10:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 11:55:49 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_cmd *cmd)
{
	int			i;
	t_expand	*current;
	t_expand	*prev;

	if (!cmd->av[1])
		return (0);
	i = 1;
	while (cmd->av[i])
	{
		if (cmd->av[i][0] == '-')
			return (ft_perror("unset: options are not allowed\n"));
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
