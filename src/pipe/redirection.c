/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:00 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/29 11:13:18 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc(t_cmd *cmd, t_mem *collector)
{
	int		tmp_fd;
	char	*line;
	char	*joined;

	char *id = ft_itoa(getpid());
	unlink(id);
	tmp_fd = open(id, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		(ft_perror("cannot open tmp file"), cleanup_child(collector), exit(1));
	joined = ft_strjoin(cmd->delimiter, "\n");
	if (!joined)
		(close(tmp_fd), unlink(id), cleanup_child(collector), exit(1));
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
		(ft_perror("cannot open tmp file"), cleanup_child(collector), exit(1));
	if (dup2(tmp_fd, STDIN_FILENO) == -1)
		(perror("dup2 heredoc"), close(tmp_fd), unlink(id), cleanup_child(collector), exit(1));
	close(tmp_fd);
	unlink(id);
}

static void	redirect_input(char **infiles)
{
	int	fd;
	int	i = 0;

	while (infiles && infiles[i])
	{
		fd = open(infiles[i], O_RDONLY);
		if (fd == -1)
			(perror(infiles[i]), exit(1));
		if (!infiles[i + 1])
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				(perror("dup2 infiles"), exit(1));
		}
		close(fd);
		i++;
	}
}

static void	redirect_output(t_cmd *cmd)
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
			(perror(cmd->outfiles[i]), exit(1));
		if (!cmd->outfiles[i + 1])
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				(perror("dup2 outfiles"), exit(1));
		}
		close(fd);
		i++;
	}
}

void	redirection(t_cmd *cmd, t_mem *collector)
{
	if (cmd->heredoc)
		heredoc(cmd, collector);
	if(cmd->infiles)
		redirect_input(cmd->infiles);
	if (cmd->outfiles)
		redirect_output(cmd);
}
