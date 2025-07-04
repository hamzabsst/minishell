/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:57:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/04 15:47:49 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_signals(int *status, int sig)
{
	if (sig == SIGINT)
	{
		*status = 130;
		ft_putstr_fd("\n", 1);
	}
	else if (sig == SIGQUIT)
	{
		*status = 131;
		ft_putstr_fd("Quit (core dumped)\n", 1);
	}
	else if (sig == SIGSEGV)
	{
		*status = 139;
		ft_putstr_fd("Segmentation fault (core dumped)\n", 1);
	}
	else
		*status = 128 + sig;
}

static void	wait_for_all(int *status, pid_t last_pid)
{
	pid_t	pid;
	int		tmp;
	int		sig;

	*status = 0;
	pid = 1;
	while (pid > 0)
	{
		pid = wait(&tmp);
		if (pid == last_pid)
		{
			if (WIFSIGNALED(tmp))
			{
				sig = WTERMSIG(tmp);
				setup_signals(status, sig);
			}
			else
				*status = WEXITSTATUS(tmp);
		}
	}
}

static void	init_pipe(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			our_error("pipe: command failed\n");
			exit(1);
		}
	}
	else
	{
		pipe_fd[0] = 0;
		pipe_fd[1] = 1;
	}
}

static void	exec_cmds(t_cmd *cmd, pid_t *last_pid, int *fd_in)
{
	pid_t	pid;
	int		pipe_fd[2];

	while (cmd)
	{
		init_pipe(cmd, pipe_fd);
		pid = init_childs(cmd, *fd_in, pipe_fd);
		if (pid == -1)
			exit(1);
		if (*fd_in != 0)
			close(*fd_in);
		if (cmd->next)
		{
			close(pipe_fd[1]);
			*fd_in = pipe_fd[0];
		}
		else
		{
			*fd_in = 0;
			*last_pid = pid;
		}
		cmd = cmd->next;
	}
}

int	init_exec(t_cmd *cmd)
{
	pid_t	last_pid;
	int		fd_in;
	int		status;

	fd_in = 0;
	last_pid = 0;
	exec_cmds(cmd, &last_pid, &fd_in);
	wait_for_all(&status, last_pid);
	return (status);
}
