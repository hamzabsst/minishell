/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:57:03 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/04 15:47:49 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (our_error("pwd: getcwd failed\n"));
	ft_printf("%s\n", cwd);
	free(cwd);
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

	newline = 1;
	i = 1;
	handle_flag(cmd->av, &i, &newline);
	while (cmd->av[i])
	{
		ft_putstr_fd(cmd->av[i], 1);
		if (cmd->av[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
