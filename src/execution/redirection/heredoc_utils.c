/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:03:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/30 15:35:27 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_heredoc(t_cmd *cmd, char *filepath, int *fd)
{
	int		stdin_backup;

	if (!cmd || !cmd->delimiter)
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

char	*remove_quotes(t_cmd *cmd)
{
	size_t	i;
	size_t	j;
	char	*result;
	char	quote_char;

	if (!cmd || !cmd->delimiter)
		return (NULL);
	result = ft_malloc(cmd->gc , ft_strlen(cmd->delimiter) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < ft_strlen(cmd->delimiter))
	{
		if (cmd->delimiter[i] == '\'' || cmd->delimiter[i] == '"')
		{
			quote_char = cmd->delimiter[i++];
			while (i < ft_strlen(cmd->delimiter) && cmd->delimiter[i] != quote_char)
				result[j++] = cmd->delimiter[i++];
			if (i < ft_strlen(cmd->delimiter))
				i++;
		}
		else
			result[j++] = cmd->delimiter[i++];
	}
	result[j] = '\0';
	return (our_strdup(cmd->gc, result));
}

int	compare_delimiter(const char *line, const char *delimiter)
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

int	tmp_to_heredoc(t_cmd *cmd)
{
	int	fd;

	if (!cmd->heredoc)
		return (1);
	if (cmd->heredoc_processed)
		return (0);
	fd = open(cmd->heredoc, O_RDONLY);
	if (fd < 0)
	{
		unlink(cmd->heredoc);
		return (our_perror("minishell: redirection from tmp file failed\n"));
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		unlink(cmd->heredoc);
		return (close(fd), our_perror("minishell: dup2 failed\n"));
	}
	close(fd);
	unlink(cmd->heredoc);
	cmd->heredoc_processed = 1;
	return (0);
}

static pid_t	get_pid_from_proc(void)
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
	const char	*prefix;
	char		*num_str;
	long		combined_num;
	pid_t		pid;

	prefix = "/tmp/.minishell_heredoc_";
	if (ft_strlen(prefix) >= size - 1)
		return ;
	pid = get_pid_from_proc();
	if (pid < 0)
		pid = 1;
	combined_num = pid + index;
	num_str = ft_itoa(combined_num);
	if (!num_str)
		return ;
	if (ft_strlen(prefix) + ft_strlen(num_str) >= size)
	{
		free(num_str);
		return ;
	}
	ft_strlcpy(dest, prefix, size);
	ft_strlcat(dest, num_str, size);
	free(num_str);
}
