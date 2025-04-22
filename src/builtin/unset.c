/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:10:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/22 10:00:35 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	filter_env(char **env, char **new_env, char *var, int *found)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (env[i])
	{
		if (!found && ft_strcmp(env[i], var) == 0
			&& env[i][ft_strlen(var)] == '=')
		{
			free(env[i]);
			*found = 1;
		}
		else
			new_env[j++] = env[i];
		i++;
	}
	new_env[j] = NULL;
}

static int	unset_env(char ***env_ptr, char *var)
{
	char	**env;
	char	**new_env;
	int		i;
	int		found;

	env = *env_ptr;
	i = 0;
	found = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * i);
	if (!new_env)
		return (1);
	filter_env(env, new_env, var, &found);
	if (found)
		free(*env_ptr);
	*env_ptr = new_env;
	return (0);
}

int	builtin_unset(char **av, char ***env)
{
	int	i;

	if (!av[1])
		return (0);
	i = 1;
	while (av[i])
	{
		if (av[i][0] == '-')
		{
			write(2, "unset: options are not allowed\n", 32);
			return (1);
		}
		unset_env(env, av[i]);
		i++;
	}
	return (0);
}
