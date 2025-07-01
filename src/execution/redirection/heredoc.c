/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:25:52 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/01 14:47:01 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_var = 130;
	ft_putstr_fd("\n", 1);
	close(STDIN_FILENO);
}

int	clean_heredoc(int fd, const char *path, int in, void (*handler)(int))
{
	if (fd >= 0)
		close(fd);
	if (path)
		unlink(path);
	if (in >= 0)
		restore_io(in, -1);
	if (handler)
		signal(SIGINT, handler);
	return (1);
}

static int	read_heredoc(int fd, char *clean_delim, int in_backup, char *path)
{
	char	*line;
	void	(*handler)(int);

	handler = signal(SIGINT, handle_heredoc_sigint);
	while (1)
	{
		if (g_var == 130)
			return (clean_heredoc(fd, path, in_backup, handler));
		line = readline("heredoc> ");
		if (g_var == 130)
			return (free(line), clean_heredoc(fd, path, in_backup, handler));
		if (!line)
		{
			our_perror("\nwarning: here-document delimited by end-of-file\n");
			break ;
		}
		if (compare_delimiter(line, clean_delim))
		{
			free(line);
			break ;
		}
		add_history(line);
		ft_putendl_fd(line, fd);
		free(line);
	}
	restore_io(in_backup, -1);
	signal(SIGINT, handler);
	return (0);
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
		return (clean_heredoc(fd, filepath, stdin_backup, NULL), NULL);
	if (read_heredoc(fd, clean_delim, stdin_backup, filepath))
		return (NULL);
	close(fd);
	return (our_strdup(cmd->gc, filepath));
}
