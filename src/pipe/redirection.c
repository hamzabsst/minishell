/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:00 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/22 11:32:41 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	in_from_heredoc(void)
{
	int	fd;

	fd = open(".heredoc_tmp", O_RDONLY);
	if (fd == -1)
	{
		perror("heredoc open");
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		exit(1);
	}
	close(fd);
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
		in_from_heredoc();
	else if (cmd->infile)
		in_from_file(cmd->infile);
	if (cmd->outfile)
		out_to_file(cmd->outfile, cmd->append);
}

