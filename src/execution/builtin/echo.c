/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:57:03 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/13 13:26:15 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_cmd *cmd)
{
	char	*cwd;
	char	*full;

	cwd = get_cwd();
	if (!cwd)
		return (1);
	full = our_strjoin(cmd->gc, cwd, "\n");
	free(cwd);
	ft_putstr_fd(full, 1);
	return (0);
}

static void	handle_flag(char **av, int *i, int *newline)
{
	int		valid;
	char	*ptr;

	while (av[*i] && !ft_strncmp(av[*i], "-n", 2))
	{
		ptr = av[*i] + 1;
		valid = 1;
		while (*ptr)
		{
			if (*ptr != 'n')
			{
				valid = 0;
				break ;
			}
			ptr++;
		}
		if (!valid)
			break ;
		*newline = 0;
		(*i)++;
	}
}

int	builtin_echo(t_cmd *cmd)
{
	int		i;
	int		newline;
	char	*result;

	newline = 1;
	i = 1;
	result = our_strdup(cmd->gc, "");
	handle_flag(cmd->av, &i, &newline);
	while (cmd->av[i])
	{
		result = our_strjoin(cmd->gc, result, cmd->av[i]);
		if (cmd->av[i + 1])
			result = our_strjoin(cmd->gc, result, " ");
		i++;
	}
	if (newline)
		result = our_strjoin(cmd->gc, result, "\n");
	ft_putstr_fd(result, 1);
	return (0);
}
