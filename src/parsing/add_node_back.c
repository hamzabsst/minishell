/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_node_back.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 09:50:20 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/03 21:29:28 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*is_last_cmd(t_cmd *cmd)
{
	if (cmd == NULL)
		return (NULL);
	while (cmd->next)
		cmd = cmd->next;
	return (cmd);
}

void	add_cmd_back(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*last;

	if (*cmd == NULL)
	{
		*cmd = new;
		return ;
	}
	last = is_last_cmd(*cmd);
	last->next = new;
}
