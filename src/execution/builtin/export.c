/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/27 18:12:53 by hbousset         ###   ########.fr       */
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

static int	print_export(t_cmd *cmd)
{
	char	**env_array;
	char	*equal;
	int		i;

	env_array = env_to_array(cmd);
	if (!env_array)
		return (1);
	sort_env(env_array);
	i = 0;
	while (env_array[i])
	{
		equal = ft_strchr(env_array[i], '=');
		if (equal)
		{
			*equal = '\0';
			ft_printf("declare -x %s=\"%s\"\n", env_array[i], equal + 1);
			*equal = '=';
		}
		else
			ft_printf("declare -x %s\n", env_array[i]);
		i++;
	}
	return (0);
}

static int	identifier(const char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	s++;
	while (*s && *s != '=' && *s != '+')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	if (*s == '+')
	{
		if (*(s + 1) != '=')
			return (0);
	}
	return (1);
}

int	builtin_export(t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!cmd->av[1])
		return (print_export(cmd));
	while (cmd->av[i])
	{
		if (!identifier(cmd->av[i]))
			ft_perror("export: `", cmd->av[i],
				"': not a valid identifier\n", cmd->gc);
		else
			process_av(cmd, cmd->av[i]);
		i++;
	}
	return (0);
}
