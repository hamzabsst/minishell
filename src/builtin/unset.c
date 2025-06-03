/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:10:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/03 22:12:15 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	rm_var(char *env_entry, char *var, int var_len)
{
	if (ft_strncmp(env_entry, var, var_len) == 0 && env_entry[var_len] == '=')
		return (1);
	return (0);
}

static int	unset_env(t_mem *collector, char ***env_ptr, char *var)
{
	char	**env;
	char	**new_env;
	int		i;
	int		j;
	int		var_len;

	env = *env_ptr;
	var_len = ft_strlen(var);
	i = 0;
	while (env[i])
		i++;
	new_env = ft_malloc(collector, sizeof(char *) * (i + 1));
	if (!new_env)
		return (1);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (!rm_var(env[i], var, var_len))
			new_env[j++] = env[i];
		i++;
	}
	new_env[j] = NULL;
	*env_ptr = new_env;
	return (0);
}

int	builtin_unset(char **av, char ***env, t_mem *collector)
{
	int	i;

	if (!av[1])
		return (0);
	i = 1;
	while (av[i])
	{
		if (av[i][0] == '-')
			return (ft_perror("unset: options are not allowed\n"));
		unset_env(collector, env, av[i]);
		i++;
	}
	return (0);
}
