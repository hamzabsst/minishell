/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:57:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/28 09:33:21 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_cmd *cmd, char **env, int in, int out, t_mem *manager)
{
	int	exit_code;

	if (dup2(in, STDIN_FILENO) == -1 || dup2(out, STDOUT_FILENO) == -1)
		(perror("dup2"), cleanup_child(manager), exit(1));
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	redirection(cmd, manager);
	if (builtin(cmd->av[0]) && !cmd->next)
	{
		exit_code = exec_builtin(cmd, &env, manager);
		cleanup_child(manager);
		exit(exit_code);
	}
	exec_cmd(cmd, env, manager);
}

static void	handle_pipe(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
			(perror("pipe"), exit(1));
	}
	else
		pipe_fd[1] = STDOUT_FILENO;
}

static void	wait_for_all(int *status, pid_t last_pid)
{
	pid_t	pid;
	int		tmp;

	*status = 0;
	pid = 0;
	while ((pid = wait(&tmp)) > 0)
	{
		if (pid == last_pid)
			*status = tmp;
	}
}

int	ft_exec(t_cmd *cmd, char **env, t_mem *manager)
{
	int		pipe_fd[2];
	int		fd_in;
	pid_t	pid;
	pid_t	last_pid;
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
			exec_child(cmd, env, fd_in, pipe_fd[1], manager);
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
