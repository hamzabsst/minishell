/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:03:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/02 01:52:03 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*remove_quotes(t_cmd *cmd)
{
	char	*result;
	size_t	i;
	size_t	j;

	result = ft_malloc(cmd->gc, ft_strlen(cmd->delimiter) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < ft_strlen(cmd->delimiter))
	{
		if (cmd->delimiter[i] == '\'' || cmd->delimiter[i] == '"')
		{
			i++;
			while (i < ft_strlen(cmd->delimiter)
				&& cmd->delimiter[i] != cmd->delimiter[i - 1])
				result[j++] = cmd->delimiter[i++];
			if (i < ft_strlen(cmd->delimiter))
				i++;
		}
		else
			result[j++] = cmd->delimiter[i++];
	}
	result[j] = '\0';
	return (result);
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

void	generate_filename(char *dest, int index)
{
	const char	*prefix;
	char		*num_str;
	long		combined_num;
	pid_t		pid;

	prefix = "/tmp/.minishell_heredoc_";
	pid = get_pid_from_proc();
	if (pid < 0)
		pid = 1;
	combined_num = pid + index;
	num_str = ft_itoa(combined_num);
	if (!num_str)
		return ;
	ft_strlcpy(dest, prefix, 128);
	ft_strlcat(dest, num_str, 128);
	free(num_str);
}
