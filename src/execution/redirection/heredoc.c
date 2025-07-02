/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:25:52 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/02 00:48:17 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_var = 2;
	ft_putstr_fd("\n", 1);
	close(STDIN_FILENO);
}

static void	clean_heredoc(int fd, const char *path, int in)
{
	if (fd >= 0)
		close(fd);
	if (path)
		unlink(path);
	if (in >= 0)
		restore_io(in, -1);
}

static int	read_heredoc(int fd, char *clean_delim, int in_backup, int *exit_code)
{
	char	*line;
	void	(*old_handler)(int);

	old_handler = signal(SIGINT, handle_heredoc_sigint);
	while (1)
	{
		if (g_var == 2)
			return (*exit_code = 130, g_var = 0, signal(SIGINT, old_handler), 1);
		line = readline("heredoc> ");
		if (g_var == 2)
			return (*exit_code = 130, g_var = 0, free(line), signal(SIGINT, old_handler), 1);
		if (!line)
			return (our_perror("warning: here-document delimited by end-of-file\n"),
				restore_io(in_backup, -1), signal(SIGINT, old_handler), g_var = 0, 0);
		if (!ft_strcmp(line, clean_delim))
			return (free(line), restore_io(in_backup, -1), signal(SIGINT, old_handler), g_var = 0, 0);
		add_history(line);
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static int	setup_heredoc(t_cmd *cmd, char *filepath, int *fd)
{
	int	stdin_backup;

	if (!cmd || !cmd->delimiter)
		return (-1);
	unlink(filepath);
	*fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (*fd < 0)
		return (our_perror("heredoc: failed to create temp file"), -1);
	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (clean_heredoc(*fd, filepath, -1), -1);
	return (stdin_backup);
}

char	*heredoc(t_cmd *cmd, int *index, int *exit_code)
{
	char	*clean_delim;
	char	filepath[128];
	int		fd;
	int		stdin_backup;

	generate_filename(filepath, (*index)++);
	if ((stdin_backup = setup_heredoc(cmd, filepath, &fd)) == -1)
		return (NULL);
	if (!(clean_delim = remove_quotes(cmd)))
		return (clean_heredoc(fd, filepath, stdin_backup), NULL);
	if (read_heredoc(fd, clean_delim, stdin_backup, exit_code))
		return (clean_heredoc(fd, filepath, stdin_backup), NULL);
	return (close(fd), our_strdup(cmd->gc, filepath));
}
