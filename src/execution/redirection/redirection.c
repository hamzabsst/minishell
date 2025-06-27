/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:00 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/27 14:40:16 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_input(t_cmd *cmd)
{
	int	fd;
	int	i;

	if (!cmd->infiles)
		return (0);
	i = 0;
	while (cmd->infiles[i])
	{
		fd = open(cmd->infiles[i], O_RDONLY);
		if (fd == -1)
			return (ft_perror(cmd->infiles[i], ": Error opening this file", "\n", cmd->gc));
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
			return (ft_perror(cmd->outfiles[i], ": Error opening this file", "\n", cmd->gc));
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
	fd = open(cmd->infiles[last], O_RDONLY);
	if (fd == -1)
		return (perror(cmd->infiles[last]), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), ft_perror("minishell", ": Error redirecting to this file", NULL, cmd->gc));
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
	fd = open(cmd->outfiles[last], flags, 0644);
	if (fd == -1)
		return (perror(cmd->outfiles[last]), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), ft_perror("minishell", ": Error redirecting to this file", NULL, cmd->gc));
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
