/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:03:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/26 09:59:03 by hbousset         ###   ########.fr       */
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

int	tmp_to_heredoc(t_cmd *cmd)
{
	int	fd;

	if (!cmd->heredoc)
		return (1);
	fd = open(cmd->heredoc, O_RDONLY);
	if (fd < 0)
	{
		unlink(cmd->heredoc);
		return (our_perror("minishell: input redirection failed\n"));
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		unlink(cmd->heredoc);
		return (close(fd), our_perror("minishell: dup2 failed\n"));
	}
	close(fd);
	unlink(cmd->heredoc);
	return (0);
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
	g_var = 0;
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
	const char	*prefix = "/tmp/.minishell_heredoc_";
	pid_t		pid;
	long		combined_num;
	char		*num_str;

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
