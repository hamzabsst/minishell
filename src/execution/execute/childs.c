/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 02:23:40 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/07 15:25:30 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cmd(t_cmd *cmd)
{
	char	*path;
	char	**env;
	int		exit_code;

	env = env_to_array(cmd);
	if (!cmd->av || !cmd->av[0])
		(ft_free_all(cmd->gc), exit(0));
	path = get_cmd_path(cmd, env);
	if (!path)
	{
		exit_code = cmd->exit_code;
		(ft_free_all(cmd->gc), exit(exit_code));
	}
	execve(path, cmd->av, env);
	ft_error("execve", strerror(errno), cmd->gc);
	ft_free_all(cmd->gc);
	exit(126);
}

//child process should inherite parent signal
static void	exec_child(t_cmd *cmd, int in, int out)
{
	int	exit_code;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		(our_error("dup2: command failed\n"), ft_free_all(cmd->gc), exit(1));
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	if (redirection(cmd))
		(ft_free_all(cmd->gc), exit(1));
	if (builtin(cmd->av[0]))
	{
		cmd->forked = 1;
		cmd->exit_code = exec_builtin(cmd);
		exit_code = cmd->exit_code;
		ft_free_all(cmd->gc);
		exit(exit_code);
	}
	exec_cmd(cmd);
}

pid_t	init_childs(t_cmd *cmd, int fd_in, int *pipe_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		our_error("fork failed\n");
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
		exec_child(cmd, fd_in, pipe_fd[1]);
	}
	return (pid);
}
