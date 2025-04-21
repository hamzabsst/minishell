/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:10:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 10:35:09 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_env(char ***env_ptr, const char *key)
{
	char	**env;
	char	**new_env;

	int (i), (j), (found);
	env = *env_ptr;
	i = 0;
	j = 0;
	found = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * i);
	if (!new_env)
		return (1);
	i = 0;
	while (env[i])
	{
		if (!found && ft_strcmp(env[i], key) == 0 && env[i][ft_strlen(key)] == '=')
		{
			free(env[i]);
			found = 1;
		}
		else
			new_env[j++] = env[i];
		i++;
	}
	new_env[j] = NULL;
	if (found)
		free(*env_ptr);
	*env_ptr = new_env;
	return (0);
}

int	builtin_unset(char **argv, char ***env_ptr)
{
	int	i;

	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '-')
		{
			write(2, "unset: options are not allowed\n", 32);
			return (1);
		}
		unset_env(env_ptr, argv[i]);
		i++;
	}
	return (0);
}
