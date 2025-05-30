/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:00 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/30 21:25:02 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc(t_cmd *cmd, t_mem *collector)
{
	int		tmp_fd;
	char	*line;
	char	*joined;

	char *id = ft_itoa(getpid());
	unlink(id);
	tmp_fd = open(id, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		return (ft_perror("cannot open tmp file"), cleanup_child(collector), 1);
	joined = ft_strjoin(cmd->delimiter, "\n");
	if (!joined)
		return (close(tmp_fd), unlink(id), cleanup_child(collector),1);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line || !ft_strcmp(line, cmd->delimiter) || !ft_strcmp(line, joined))
		{
			free(line);
			break ;
		}
		(ft_putstr_fd(line, tmp_fd), free(line));
	}
	get_next_line(-1);
	(free(joined), close(tmp_fd));
	tmp_fd = open(id, O_RDONLY);
	if (tmp_fd < 0)
		return (ft_perror("cannot open tmp file"), cleanup_child(collector), 1);
	if (dup2(tmp_fd, STDIN_FILENO) == -1)
		return (perror("dup2 heredoc"), close(tmp_fd), unlink(id), cleanup_child(collector), 1);
	close(tmp_fd);
	unlink(id);
	return (0);
}

static int	input_redir(char **infiles)
{
	int	fd;
	int	last_index;

	last_index = 0;
	while (infiles[last_index])
		last_index++;
	last_index--;
	fd = open(infiles[last_index], O_RDONLY);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 infiles");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	output_redir(t_cmd *cmd)
{
	int	fd;
	int	i;
	int	last_index;
	int	flags;

	last_index = 0;
	i = 0;
	while (cmd->outfiles[last_index])
		last_index++;
	last_index--;
	flags = cmd->append_flags[last_index];
	if (flags == 1)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(cmd->outfiles[last_index], flags, 0644);
	if (fd == -1)
	{
		perror(cmd->outfiles[last_index]);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 outfiles");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

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
		{
			perror(infiles[i]);
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

int	redirection(t_cmd *cmd, t_mem *collector)
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
	if (cmd->heredoc)
	{
		if (heredoc(cmd, collector))
			return (1);
	}
	else if (cmd->infiles)
	{
		if (input_redir(cmd->infiles))
			return (1);
	}
	if (cmd->outfiles)
	{
		if (output_redir(cmd))
			return (1);
	}
	return (0);
}
