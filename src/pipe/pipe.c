/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:57:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 14:36:52 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_in_paths(char *cmd, char **paths)
{
	char	*full;
	int		i = 0;

	while (paths && paths[i])
	{
		full = malloc(ft_strlen(paths[i]) + ft_strlen(cmd) + 2);
		if (!full)
			return (NULL);
		ft_strcpy(full, paths[i]);
		ft_strcat(full, "/");
		ft_strcat(full, cmd);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	**get_path(char **env)
{
	int	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	return (ft_split(env[i] + 5, ':'));
}

char	*get_cmd_path(t_cmd *cmd, char **env)
{
	if (!cmd || !cmd->av || !cmd->av[0])
		return (NULL);

	if (ft_strchr(cmd->av[0], '/'))
	{
		if (access(cmd->av[0], X_OK) == 0)
			return (ft_strdup(cmd->av[0]));
		perror(cmd->av[0]);
		exit(126);
	}

	char **paths = get_path(env);
	if (!paths)
		return (NULL);

	char *resolved = find_in_paths(cmd->av[0], paths);
	ft_free(paths);
	return (resolved);
}

void	exec_cmd(t_cmd *cmd, char **env)
{
	if (!cmd->av || !cmd->av[0])
	{
		write(2, "empty command\n", 14);
		exit(127);
	}
	char *path = get_cmd_path(cmd, env);
	if (!path)
	{
		ft_putstr_fd(cmd->av[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, cmd->av, env);
	perror("execve");
	free(path);
	exit(1);
}

static void	exec_child(t_cmd *cmd, char **env, int fd_in, int fd_out)
{
	if (dup2(fd_in, STDIN_FILENO) == -1 || dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	if (builtin(cmd->av[0]) && !cmd->next)
		exit(exec_builtin(cmd, &env));
	exec_cmd(cmd, env);
	exit(1);
}

int	exec_pipeline(t_cmd *cmd, char **env)
{
	int		pipe_fd[2];
	int		fd_in = STDIN_FILENO;
	pid_t	pid;
	int		status;

	while (cmd)
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
	while (wait(&status) > 0)
	;
	return (WEXITSTATUS(status));
}
