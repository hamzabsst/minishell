/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:57:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 10:58:50 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pipeline(t_cmd *cmd, char **envp)
{
	int		pipe_fd[2];
	int		fd_in = STDIN_FILENO;
	pid_t	pid;
	int		status;
	char	**env = envp;

	while (cmd)
	{
		if (cmd->next)
			pipe(pipe_fd);
	}
}

