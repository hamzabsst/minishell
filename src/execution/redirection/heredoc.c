/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:25:52 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/20 14:15:19 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quoted_delimiter(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 2)
		return (0);
	return ((s[0] == '\'' && s[len - 1] == '\'') || (s[0] == '"' && s[len - 1] == '"'));
}

char	*remove_quotes(t_mem *gc, const char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'') || (s[0] == '"' && s[len - 1] == '"')))
		return (our_strndup(gc, (char *)s + 1, len - 2, 0, 0));
	return (our_strdup(gc, s));
}

int	get_pid_from_proc(void)
{
	int		fd;
	char	buffer[256];
	int		bytes_read;
	int		pid;
	int		i;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes_read <= 0)
		return (-1);
	buffer[bytes_read] = '\0';
	pid = 0;
	i = 0;
	while (i < bytes_read && buffer[i] >= '0' && buffer[i] <= '9')
	{
		pid = pid * 10 + (buffer[i] - '0');
		i++;
	}
	return (pid);
}

void	generate_filename(char *dest, size_t size, int index)
{
	const char	*prefix = "/tmp/minishell_heredoc_";
	size_t		prefix_len;
	size_t		i;
	char		num_str[32];
	int			num_len;
	pid_t		pid;
	long		combined_num;

	prefix_len = ft_strlen(prefix);
	if (prefix_len >= size - 1)
		return ;
	i = 0;
	while (i < prefix_len && i < size - 1)
	{
		dest[i] = prefix[i];
		i++;
	}
	pid = get_pid_from_proc();
	if (pid < 0)
		pid = 1;
	combined_num = (long)pid * 10000 + index;
	num_len = 0;
	if (combined_num == 0)
		num_str[num_len++] = '0';
	else
	{
		while (combined_num > 0 && num_len < 31)
		{
			num_str[num_len++] = '0' + (combined_num % 10);
			combined_num /= 10;
		}
	}
	while (num_len > 0 && i < size - 1)
		dest[i++] = num_str[--num_len];
	dest[i] = '\0';
}

static int	create_heredoc_file(const char *filepath)
{
	int	fd;

	unlink(filepath);
	fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (ft_perror("heredoc: failed to create temp file"), -1);
	return (fd);
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
//? dont forget to hide the file and unlink it after you done
char	*heredoc(t_cmd *cmd, t_mem *gc, int *index)
{
	char		*clean_delim;
	char		filepath[64];
	int			fd;
	char		*line;
	char		*filepath_copy;

	if (!cmd || !cmd->delimiter)
		return (NULL);
	clean_delim = remove_quotes(gc, cmd->delimiter);
	if (!clean_delim)
		return (NULL);
	generate_filename(filepath, sizeof(filepath), (*index)++);
	fd = create_heredoc_file(filepath);
	if (fd < 0)
		return (NULL);
	g_sig = 1;
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_perror("\nminishell: warning: here-document delimited by end-of-file\n");
			break;
		}
		if (compare_delimiter(line, clean_delim))
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	g_sig = 0;
	close(fd);
	filepath_copy = our_strdup(gc, filepath);
	return (filepath_copy);
}
