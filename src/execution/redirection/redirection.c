/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:00 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/20 11:44:16 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_input(char **infiles)
{
	int	fd;
	int	i;

	if (!infiles)
		return (0);
	i = 0;
	while (infiles[i])
	{
		fd = open(infiles[i], O_RDONLY);
		if (fd == -1)
			return (perror(infiles[i]), 1);
		close(fd);
		i++;
	}
	return (0);
}

static int	valid_output(t_cmd *cmd)
{
	int	fd;
	int	i = 0;

	while (cmd->outfiles && cmd->outfiles[i])
	{
		int flags = cmd->append_flags[i];
		if (flags == 1)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(cmd->outfiles[i], flags, 0644);
		if (fd == -1)
		{
			perror(cmd->outfiles[i]);
			return (1);
		}
		close(fd);
		i++;
	}
	return (0);
}

static int	input_redir(char **infiles)
{
	int	fd;
	int	last;

	last = 0;
	while (infiles[last])
		last++;
	last--;
	fd = open(infiles[last], O_RDONLY);
	if (fd == -1)
		return (perror(infiles[last]), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2 infiles"), close(fd), 1);
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
		return (perror("dup2 outfiles"), close(fd), 1);
	close(fd);
	return (0);
}
static int	input_redir_file(char *filename)
{
	int	fd;

	if (!filename)
		return (1);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_perror("minishell: input redirection failed"));
	if (dup2(fd, STDIN_FILENO) < 0)
		return (close(fd), ft_perror("minishell: dup2 failed"));
	close(fd);
	return (0);
}

int	redirection(t_cmd *cmd)
{
	if (cmd->infiles)
	{
		if (valid_input(cmd->infiles))
			return (1);
	}
	if (cmd->outfiles)
	{
		if (valid_output(cmd))
			return (1);
	}
	if (cmd->outfiles)
	{
		if (output_redir(cmd))
			return (1);
	}
	if (cmd->heredoc)
	{
		if (input_redir_file(cmd->heredoc))
			return (1);
	}
	else if (cmd->infiles)
	{
		if (input_redir(cmd->infiles))
			return (1);
	}
	return (0);
}
