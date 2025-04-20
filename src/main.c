/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/20 14:50:42 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	**dup_env(char **env)
{
	int		i;
	int		n;
	char	**copy;

	n = 0;
	while (env[n])
		n++;
	copy = malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

int main(int ac, char **av, char **env)
{
	t_cmd	cmd;
	char	**g_env;

	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		exit(1);
	}
	cmd.argv = (char *[]){"cd", "../minishell/src", NULL};
	cmd.next = NULL;
	g_env = dup_env(env);
	exec_builtin(&cmd, g_env);
	cmd.argv = (char *[]){"pwd", NULL};
	exec_builtin(&cmd, g_env);
	return (0);
}

