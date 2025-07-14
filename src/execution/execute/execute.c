/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:35 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/14 18:21:05 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin(char *av)
{
	if (!av)
		return (0);
	return (!ft_strcmp(av, "echo")
		|| !ft_strcmp(av, "cd")
		|| !ft_strcmp(av, "pwd")
		|| !ft_strcmp(av, "export")
		|| !ft_strcmp(av, "unset")
		|| !ft_strcmp(av, "env")
		|| !ft_strcmp(av, "exit"));
}

int	exec_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->av[0], "echo"))
		return (builtin_echo(cmd));
	else if (!ft_strcmp(cmd->av[0], "pwd"))
		return (builtin_pwd(cmd));
	else if (!ft_strcmp(cmd->av[0], "cd"))
		return (builtin_cd(cmd));
	else if (!ft_strcmp(cmd->av[0], "env"))
		return (builtin_env(cmd));
	else if (!ft_strcmp(cmd->av[0], "unset"))
		return (builtin_unset(cmd));
	else if (!ft_strcmp(cmd->av[0], "exit"))
		return (builtin_exit(cmd));
	else if (!ft_strcmp(cmd->av[0], "export"))
		return (builtin_export(cmd));
	return (1);
}

void	restore_io(int in_copy, int out_copy)
{
	if (in_copy != -1)
		(dup2(in_copy, 0), close(in_copy));
	if (out_copy != -1)
		(dup2(out_copy, 1), close(out_copy));
}

static int	backup_io(int *in_copy, int *out_copy)
{
	int	temp_fd;

	*in_copy = dup(STDIN_FILENO);
	while (*in_copy != -1 && *in_copy < 3)
	{
		temp_fd = *in_copy;
		*in_copy = dup(STDIN_FILENO);
		close(temp_fd);
	}
	*out_copy = dup(STDOUT_FILENO);
	while (*out_copy != -1 && *out_copy < 3)
	{
		temp_fd = *out_copy;
		*out_copy = dup(STDOUT_FILENO);
		close(temp_fd);
	}
	return (0);
}

int	process_command(t_cmd *cmd)
{
	if (builtin(cmd->av[0]) && !cmd->next)
	{
		backup_io(&cmd->in_copy, &cmd->out_copy);
		if (redirection(cmd))
			return (restore_io(cmd->in_copy, cmd->out_copy), our_error("redirection failed\n"));
		cmd->exit_code = exec_builtin(cmd);
		restore_io(cmd->in_copy, cmd->out_copy);
	}
	else
	{
		cmd->exit_code = init_exec(cmd);
		restore_io(cmd->in_copy, cmd->out_copy);
	}
	return (cmd->exit_code);
}
