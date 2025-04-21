/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 10:35:05 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_env(char **env)
{
	int		i = 0;
	char	**copy;
	char	*equal_pos;

	copy = dup_env(env);
	if (!copy)
		return;
	sort_env(copy);
	while (copy[i])
	{
		equal_pos = ft_strchr(copy[i], '=');
		if (equal_pos)
		{
			*equal_pos = '\0';
			printf("declare -x %s=\"%s\"\n", copy[i], equal_pos + 1);
			*equal_pos = '=';
		}
		else
			printf("declare -x %s\n", copy[i]);
		i++;
	}
	ft_free(copy);
}

int	builtin_export(char **argv, char **envp)
{
	if (argv[1])
	{
		write(2, "export: arguments not supported yet\n", 36);
		return (1);
	}
	print_sorted_env(envp);
	return (0);
}
