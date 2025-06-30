/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:25:52 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/30 15:07:38 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_var = 2;
	ft_putstr_fd("\n", 1);
	close(STDIN_FILENO);
}

void	clean_heredoc(int fd, const char *path, int in, void (*handler)(int))
{
	if (fd >= 0)
		close(fd);
	if (path)
		unlink(path);
	if (in >= 0)
		restore_io(in, -1);
	if (handler)
		signal(SIGINT, handler);
}

static int	setup_heredoc(t_cmd *cmd, char *filepath, int *fd)
{
	char	*clean_delim;
	int		stdin_backup;

	if (!cmd || !cmd->delimiter)
		return (-1);
	clean_delim = remove_quotes(cmd);
	if (!clean_delim)
		return (-1);
	unlink(filepath);
	*fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (*fd < 0)
		return (our_perror("heredoc: failed to create temp file"), -1);
	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (clean_heredoc(*fd, filepath, -1, NULL), -1);
	return (stdin_backup);
}

static int	read_heredoc(int fd, char *clean_delim, int in_backup, char *path)
{
	char	*line;
	void	(*handler)(int);

	handler = signal(SIGINT, handle_heredoc_sigint);
	while (1)
	{
		if (g_var == 2)
			return (clean_heredoc(fd, path, in_backup, handler), -1);
		(write(STDOUT_FILENO, "> ", 2), line = get_next_line(0));
		if (g_var == 2)
			return (get_next_line(-1), free(line),
				clean_heredoc(fd, path, in_backup, handler), -1);
		if (!line)
		{
			our_perror("\nwarning: here-document delimited by end-of-file\n");
			break ;
		}
		if (compare_delimiter(line, clean_delim))
		{
			(get_next_line(-1), free(line));
			break ;
		}
		(write(fd, line, ft_strlen(line)), free(line));
	}
	return (restore_io(in_backup, -1), signal(SIGINT, handler), 0);
}

char	*heredoc(t_cmd *cmd, int *index)
{
	char	*clean_delim;
	char	filepath[128];
	int		fd;
	int		stdin_backup;

	generate_filename(filepath, sizeof(filepath), (*index)++);
	stdin_backup = setup_heredoc(cmd, filepath, &fd);
	if (stdin_backup == -1)
		return (NULL);
	clean_delim = remove_quotes(cmd);
	if (!clean_delim)
	{
		clean_heredoc(fd, filepath, stdin_backup, NULL);
		return (NULL);
	}
	if (read_heredoc(fd, clean_delim, stdin_backup, filepath) == -1)
		return (NULL);
	close(fd);
	return (our_strdup(cmd->gc, filepath));
}
