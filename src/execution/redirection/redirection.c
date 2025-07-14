/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:00 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/14 01:25:36 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_open(const char *pathname, int flags, mode_t mode)
{
	int	fd;
	int	temp_fds[20];
	int	i;

	fd = open(pathname, flags, mode);
	if (fd == -1)
		return (-1);
	if (fd >= 20)
		return (fd);
	i = 0;
	while (fd < 20)
	{
		temp_fds[i] = fd;
		fd = dup(fd);
		if (fd == -1)
		{
			while (--i >= 0)
				close(temp_fds[i]);
			return (-1);
		}
		i++;
	}
	while (--i >= 0)
		close(temp_fds[i]);
	return (fd);
}

static int	valid_input(t_cmd *cmd)
{
	int	fd;
	int	i;

	if (!cmd->infiles)
		return (0);
	i = 0;
	while (cmd->infiles[i])
	{
		fd = open(cmd->infiles[i], O_RDONLY, 0);
		if (fd == -1)
		{
			ft_error(cmd->infiles[i], strerror(errno), cmd->gc);
			return (1);
		}
		close(fd);
		i++;
	}
	return (0);
}

static int	valid_output(t_cmd *cmd)
{
	int	fd;
	int	i;
	int	flags;

	i = 0;
	while (cmd->outfiles && cmd->outfiles[i])
	{
		flags = cmd->append_flags[i];
		if (flags == 1)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(cmd->outfiles[i], flags, 0644);
		if (fd == -1)
		{
			ft_error(cmd->outfiles[i], strerror(errno), cmd->gc);
			return (1);
		}
		close(fd);
		i++;
	}
	return (0);
}

static int	input_redir(t_cmd *cmd)
{
	int	fd;
	int	last;

	last = 0;
	while (cmd->infiles[last])
		last++;
	last--;
	fd = ft_open(cmd->infiles[last], O_RDONLY, 0);
	if (fd == -1)
	{
		ft_error(cmd->infiles[last], strerror(errno), cmd->gc);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), ft_error("dup2", strerror(errno), cmd->gc));
	close(fd);
	return (0);
}

static int	output_redir(t_cmd *cmd)
{
	int	fd;
	int	last;
	int	flags;

	last = 0;
	while (cmd->outfiles[last])
		last++;
	last--;
	flags = cmd->append_flags[last];
	if (flags == 1)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = ft_open(cmd->outfiles[last], flags, 0644);
	if (fd == -1)
	{
		ft_error(cmd->outfiles[last], strerror(errno), cmd->gc);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), ft_error("dup2", strerror(errno), cmd->gc));
	close(fd);
	return (0);
}

int	redirection(t_cmd *cmd)
{
	if (cmd->infiles && valid_input(cmd))
		return (1);
	if (cmd->outfiles && valid_output(cmd))
		return (1);
	if (cmd->outfiles && output_redir(cmd))
		return (1);
	if (cmd->heredoc)
	{
		if (tmp_to_heredoc(cmd))
			return (1);
	}
	else if (cmd->infiles)
	{
		if (input_redir(cmd))
			return (1);
	}
	return (0);
}
