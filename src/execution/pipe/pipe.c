/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:57:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/26 18:07:44 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_cmd *cmd, int in, int out)
{
	int	exit_code;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		(our_perror("dup2: command failed\n"), cleanup_child(cmd->gc), exit(1));
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	if (builtin(cmd->av[0]))
	{
		cmd->forked = 1;
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
			(our_perror("pipe: command failed\n"), exit(1));
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

static int is_all_builtins(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (!builtin(current->av[0]))
			return (0);
		current = current->next;
	}
	return (1);
}

static int exec_builtin_pipeline(t_cmd *cmd)
{
	int	exit_code;

	exit_code = 0;
	while (cmd)
	{
		exit_code = exec_builtin(cmd);
		cmd = cmd->next;
	}
	return (exit_code);
}

int ft_exec(t_cmd *cmd)
{
	pid_t	pid;
	pid_t	last_pid;
	int	 	pipe_fd[2];
	int	 	fd_in;
	int	 	status;

	if (is_all_builtins(cmd))
		return (exec_builtin_pipeline(cmd));
	fd_in = STDIN_FILENO;
	last_pid = 0;
	while (cmd)
	{
		handle_pipe(cmd, pipe_fd);
		pid = fork();
		if (pid == -1)
		{
			our_perror("fork failed\n");
			if (fd_in != STDIN_FILENO)
				close(fd_in);
			if (cmd->next && pipe_fd[1] != STDOUT_FILENO)
				close(pipe_fd[1]);
			return (1);
		}
		if (pid == 0)
		{
			if (pipe_fd[0])
				close(pipe_fd[0]);
			exec_child(cmd, fd_in, pipe_fd[1]);
		}
		if (fd_in != STDIN_FILENO)
			close(fd_in);	
		if (cmd->next)
			(close(pipe_fd[1]), fd_in = pipe_fd[0]);
		else
			(fd_in = STDIN_FILENO, last_pid = pid);
		cmd = cmd->next;
	}
	wait_for_all(&status, last_pid);
	return (status);
}
