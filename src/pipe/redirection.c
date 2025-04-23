/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:00 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/23 09:31:54 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	in_from_heredoc(t_cmd *cmd)
{
	int		tmp_fd;
	char	*line;
	char	*joined;

	unlink("/tmp/.heredoc_tmp");
	tmp_fd = open("/tmp/.here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		(ft_putstr_fd("cannot open tmp file", 2), exit(EXIT_FAILURE));
	joined = ft_strjoin(cmd->delimiter, "\n");
	if (!joined)
		(close(tmp_fd), unlink("/tmp/.here_doc_tmp"), exit(EXIT_FAILURE));
	while (1)
	{
		write(1, "heredoc> ", 9);
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
	tmp_fd = open("/tmp/.heredoc_tmp", O_RDONLY);
	if (tmp_fd < 0)
		(ft_putstr_fd("cannot open tmp file", 2), exit(EXIT_FAILURE));
	if (dup2(tmp_fd, STDIN_FILENO) == -1)
		(perror("dup2 heredoc"), exit(EXIT_FAILURE));
	close(tmp_fd);
}

static void	in_from_file(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		perror(infile);
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 infile");
		exit(1);
	}
	close(fd);
}

static void	out_to_file(char *outfile, int append)
{
	int	flags;
	int	fd;

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;

	fd = open(outfile, flags, 0644);
	if (fd == -1)
	{
		perror(outfile);
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 outfile");
		exit(1);
	}
	close(fd);
}

void	handle_redirection(t_cmd *cmd)
{
	if (cmd->heredoc)
		in_from_heredoc(cmd);
	else if (cmd->infile)
		in_from_file(cmd->infile);
	if (cmd->outfile)
		out_to_file(cmd->outfile, cmd->append);
}

