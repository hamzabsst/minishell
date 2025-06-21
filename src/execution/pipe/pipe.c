/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:57:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 11:48:44 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_cmd *cmd, int in, int out)
{
	int	exit_code;

	if (dup2(in, STDIN_FILENO) == -1 || dup2(out, STDOUT_FILENO) == -1)
		(perror("dup2"), cleanup_child(cmd->gc), exit(1));
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	if (builtin(cmd->av[0]) && !cmd->next)
	{
		exit_code = exec_builtin(cmd);
		cleanup_child(cmd->gc);
		exit(exit_code);
	}
	exec_cmd(cmd);
}

static void	handle_pipe(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
			(perror("pipe"), exit(1));
	}
	else
	{
		pipe_fd[0] = STDIN_FILENO;
		pipe_fd[1] = STDOUT_FILENO;
	}
}

static void	wait_for_all(int *status, pid_t last_pid)
{
	pid_t	pid;
	int		tmp;

	*status = 0;
	while ((pid = wait(&tmp)) > 0)
	{
		if (pid == last_pid)
			*status = tmp;
	}
}

int	ft_exec(t_cmd *cmd)
{
	int		pipe_fd[2];
	int		fd_in;
	pid_t	pid;
	pid_t	last_pid = 0;
	int		status;

	fd_in = STDIN_FILENO;
	while (cmd)
	{
		handle_pipe(cmd, pipe_fd);
		pid = fork();
		if(pid == -1)
		{
			perror("fork");
			if (fd_in != STDIN_FILENO)
				close(fd_in);
			if (cmd->next && pipe_fd[1] != STDOUT_FILENO)
				close(pipe_fd[1]);
			return (1);
		}
		if (pid == 0)
			exec_child(cmd, fd_in, pipe_fd[1]);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (cmd->next)
		{
			close(pipe_fd[1]);
			fd_in = pipe_fd[0];
		}
		else
		{
			fd_in = STDIN_FILENO;
			last_pid = pid;
		}
		cmd = cmd->next;
	}
	wait_for_all(&status, last_pid);
	return (WEXITSTATUS(status));
}
