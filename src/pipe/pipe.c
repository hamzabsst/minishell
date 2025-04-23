/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:57:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/22 11:29:49 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_cmd *cmd, char **env, int in, int out)
{
	if (dup2(in, STDIN_FILENO) == -1 || dup2(out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	handle_redirection(cmd);
	if (builtin(cmd->av[0]) && !cmd->next)
		exit(exec_builtin(cmd, &env));
	exec_cmd(cmd, env);
	exit(1);
}

static void	handle_pipe(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
	}
	else
		pipe_fd[1] = STDOUT_FILENO;
}

static void	wait_for_all(int *status)
{
	while (wait(status) > 0)
		;
}

int	exec_pipeline(t_cmd *cmd, char **env)
{
	int		pipe_fd[2];
	int		fd_in;
	pid_t	pid;
	int		status;

	fd_in = STDIN_FILENO;
	while (cmd)
	{
		handle_pipe(cmd, pipe_fd);
		pid = fork();
		if (pid == 0)
			exec_child(cmd, env, fd_in, pipe_fd[1]);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (cmd->next)
		{
			close(pipe_fd[1]);
			fd_in = pipe_fd[0];
		}
		else
			fd_in = STDIN_FILENO;
		cmd = cmd->next;
	}
	wait_for_all(&status);
	return (WEXITSTATUS(status));
}
