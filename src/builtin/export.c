/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/01 14:38:38 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	print_export(char **env)
{
	char	**copy;
	int		i;
	char	*equal;

	i = 0;
	copy = dup_env(env);
	if (!copy)
		return (1);
	sort_env(copy);
	while (copy[i])
	{
		equal = ft_strchr(copy[i], '=');
		if (equal)
		{
			*equal = '\0';
			printf("declare -x %s=\"%s\"\n", copy[i], equal + 1);
			*equal = '=';
		}
		else
			printf("declare -x %s\n", copy[i]);
		i++;
	}
	ft_free(copy);
	return (0);
}

static void	process_av(char *arg, char ***env)
{
	char	*key;
	char	*equal;

	key = find_key(arg);
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		if (*(equal - 1) == '+')
			update_env_append(env, key, equal + 1);
		else
			update_env(env, key, equal + 1);
	}
	else if (!ft_getenv(*env, key))
		update_env(env, key, "");
	free(key);
}

int	builtin_export(char **av, char ***env)
{
	int	i;

	i = 1;
	if (!av[1])
		return (print_export(*env));
	while (av[i])
	{
		if (!identifier(av[i]))
		{
			ft_perror("export: `");
			ft_perror(av[i]);
			ft_perror("': not a valid identifier\n");
		}
		else
			process_av(av[i], env);
		i++;
	}
	return (0);
}
