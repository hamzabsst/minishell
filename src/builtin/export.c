/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/28 10:00:49 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid(char *s)
{
	int	i;

	i = 0;
	if (!s || !(ft_isalpha(s[i]) || s[i] == '_'))
		return (0);
	i++;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	split_export(char *av, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(av, '=');
	if (!equal)
	{
		*key = ft_strdup(av);
		*value = NULL;
	}
	else
	{
		*key = ft_substr(av, 0, equal - av);
		*value = ft_strdup(equal + 1);
	}
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
	int		i;
	char	**copy;
	char	*equal_pos;

	i = 0;
	copy = dup_env(env);
	if (!copy)
		return (1);
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
	return (0);
}

char	*get_env_value(char **env, const char *key)
{
	int		i;
	size_t	key_len;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

int builtin_export(char **av, char ***env)
{
	int		i;
	char	*key;
	char	*value;

	i = 1;
	if (!av[1])
		return (print_export(*env));
	while (av[i])
	{
		if (!is_valid(av[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(av[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			i++;
			continue;
		}
		split_export(av[i], &key, &value);
		if (!key)
			return (1);
		if (value)
			update_env(env, key, value);
		else if (!get_env_value(*env, key))
			update_env(env, key, "");
		free(key);
		free(value);
		i++;
	}
	return (0);
}
