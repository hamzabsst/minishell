/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:25:52 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/24 11:42:59 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_quotes(t_mem *gc, const char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'')
			|| (s[0] == '"' && s[len - 1] == '"')))
		return (our_strndup(gc, (char *)s + 1, len - 2, 0, 0));
	return (our_strdup(gc, s));
}

static int	compare_delimiter(const char *line, const char *delimiter)
{
	size_t	delim_len;
	size_t	line_len;

	if (!line || !delimiter)
		return (0);
	delim_len = ft_strlen(delimiter);
	line_len = ft_strlen(line);
	if (line_len == delim_len)
		return (ft_strncmp(line, delimiter, delim_len) == 0);
	if (line_len == delim_len + 1 && line[line_len - 1] == '\n')
		return (ft_strncmp(line, delimiter, delim_len) == 0);
	return (0);
}

static int	setup_heredoc(t_cmd *cmd, t_mem *gc, char *filepath, int *fd)
{
	char	*clean_delim;
	int		stdin_backup;

	if (!cmd || !cmd->delimiter)
		return (-1);
	clean_delim = remove_quotes(gc, cmd->delimiter);
	if (!clean_delim)
		return (-1);
	unlink(filepath);
	*fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (*fd < 0)
		return (ft_perror("heredoc: failed to create temp file\n"), -1);
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
			ft_perror("\nwarning: here-document delimited by end-of-file\n");
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

char	*heredoc(t_cmd *cmd, t_mem *gc, int *index)
{
	char	*clean_delim;
	char	filepath[128];
	int		fd;
	int		stdin_backup;

	generate_filename(filepath, sizeof(filepath), (*index)++);
	stdin_backup = setup_heredoc(cmd, gc, filepath, &fd);
	if (stdin_backup == -1)
		return (NULL);
	clean_delim = remove_quotes(gc, cmd->delimiter);
	if (read_heredoc(fd, clean_delim, stdin_backup, filepath) == -1)
		return (NULL);
	close(fd);
	return (our_strdup(gc, filepath));
}
