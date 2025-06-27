/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:57:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/27 23:05:08 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_cmd *cmd, int in, int out, int in_copy, int out_copy)
{
	int	exit_code;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(in_copy);
	close(out_copy);
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		(our_perror("dup2: command failed\n"), ft_free_all(cmd->gc), exit(1));
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	redirection(cmd);
	if (builtin(cmd->av[0]))
	{
		cmd->forked = 1;
		exit_code = exec_builtin(cmd);
		ft_free_all(cmd->gc);
		exit(exit_code);
	}
	exec_cmd(cmd);
}

static void	handle_pipe(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
			(our_perror("pipe: command failed\n"), exit(1));
	}
	else
		(pipe_fd[0] = 0, pipe_fd[1] = 1);
}

static void	wait_for_all(int *status, pid_t last_pid)
{
	pid_t	pid;
	int		tmp;
	int		sig;

	*status = 0;
	while ((pid = wait(&tmp)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFSIGNALED(tmp))
			{
				sig = WTERMSIG(tmp);
				if (sig == SIGINT)
					*status = 130, write(1, "\n", 1);
				else if (sig == SIGQUIT)
					(*status = 131, ft_putstr_fd("Quit (core dumped)\n", 1));
				else
					*status = 128 + sig;
			}
			else
				*status = WEXITSTATUS(tmp);
		}
	}
}

static pid_t	create_process(t_cmd *cmd, int fd_in, int *pipe_fd, int in, int out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		our_perror("fork failed\n");
		if (fd_in != 0)
			close(fd_in);
		if (cmd->next && pipe_fd[1] != 1)
			close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (pipe_fd[0] && pipe_fd[0] != 0)
			close(pipe_fd[0]);
		exec_child(cmd, fd_in, pipe_fd[1], in, out);
	}
	return (pid);
}

int	ft_exec(t_cmd *cmd, int in, int out)
{
	pid_t	pid;
	pid_t	last_pid;
	int		pipe_fd[2];
	int		fd_in;
	int		status;

	fd_in = 0;
	last_pid = 0;
	while (cmd)
	{
		handle_pipe(cmd, pipe_fd);
		pid = create_process(cmd, fd_in, pipe_fd, in, out);
		if (pid == -1)
			return (1);
		if (fd_in != 0)
			close(fd_in);
		if (cmd->next)
			(close(pipe_fd[1]), fd_in = pipe_fd[0]);
		else
			(fd_in = 0, last_pid = pid);
		cmd = cmd->next;
	}
	wait_for_all(&status, last_pid);
	return (status);
}
