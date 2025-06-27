/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:35 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/27 22:15:44 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include"minishell.h"

void	restore_io(int in_copy, int out_copy)
{
	if (in_copy != -1)
		(dup2(in_copy, 0), close(in_copy));
	if (out_copy != -1)
		(dup2(out_copy, 1), close(out_copy));
}

static int	backup_io(int *in_copy, int *out_copy)
{
	*in_copy = dup(STDIN_FILENO);
	*out_copy = dup(STDOUT_FILENO);
	if (*in_copy == -1 || *out_copy == -1)
	{
		if (*in_copy != -1)
			close(*in_copy);
		if (*out_copy != -1)
			close(*out_copy);
		return (-1);
	}
	return (0);
}

int	process_command(t_cmd *cmd)
{
	int	in_copy;
	int	out_copy;
	int	exit_code;

	in_copy = -1;
	out_copy = -1;
	if (backup_io(&in_copy, &out_copy) == -1)
		return (our_perror("Failed to backup stdio\n"));
	if (!cmd->next && redirection(cmd) != 0)
		return (restore_io(in_copy, out_copy), 1);
	if (builtin(cmd->av[0]) && !cmd->next)
		(exit_code = exec_builtin(cmd) ,restore_io(in_copy, out_copy));
	else
	{
		g_var = 1;
		exit_code = ft_exec(cmd, in_copy, out_copy);
		if (g_var == 2)
			(g_var = 0, exit_code = 130);
		else
			g_var = 0;
		restore_io(in_copy, out_copy);
	}
	return (exit_code);
}
