/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/28 10:19:09 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(char *s)
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
	int		i, j;
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
	int		i = 0;
	char	*equal;

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

static char	*find_key(char *arg)
{
	int	len;

	len = 0;
	while (arg[len] && arg[len] != '=' && !(arg[len] == '+' && arg[len + 1] == '='))
		len++;
	return (ft_substr(arg, 0, len));
}

static char	*get_env_value(char **env, char *key)
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

static int	update_env_append(char ***env, char *key, char *value)
{
	char	*old_value;
	char	*new_value;

	old_value = get_env_value(*env, key);
	if (old_value)
		new_value = ft_strjoin(old_value, value);
	else
		new_value = ft_strdup(value);
	if (!new_value)
		return (1);
	update_env(env, key, new_value);
	free(new_value);
	return (0);
}

int	builtin_export(char **av, char ***env)
{
	int		i;
	char	*key;
	char	*equal;

	i = 1;
	if (!av[1])
		return (print_export(*env));
	while (av[i])
	{
		if (!is_valid_identifier(av[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(av[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			i++;
			continue;
		}
		key = find_key(av[i]);
		equal = ft_strchr(av[i], '=');
		if (equal)
		{
			if (*(equal - 1) == '+')
				update_env_append(env, key, equal + 1);
			else
				update_env(env, key, equal + 1);
		}
		else if (!get_env_value(*env, key))
			update_env(env, key, "");
		free(key);
		i++;
	}
	return (0);
}
