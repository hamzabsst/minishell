/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:35:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/22 10:00:19 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**dup_env(char **env)
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

static int	copy_env(char ***env, char *new_var, int count)
{
	char	**old;
	char	**new_env;
	int		j;

	old = *env;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (free(new_var), 1);
	j = 0;
	while (j < count)
	{
		new_env[j] = old[j];
		++j;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(old);
	*env = new_env;
	return (0);
}

static int	update_env(char ***env_ptr, const char *key, const char *value)
{
	char	**env;
	char	*new_var;
	int		i;

	env = *env_ptr;
	if (!env_ptr || !(env) || !key || !value)
		return (1);
	new_var = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new_var)
		return (1);
	ft_strcpy(new_var, (char *)key);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, (char *)value);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0
			&& env[i][ft_strlen(key)] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return (0);
		}
	}
	return (copy_env(env_ptr, new_var, i));
}

static int	handle_path(char *arg)
{
	if (!arg || !*arg)
		return (0);
	if (arg[0] == '~' || arg[0] == '-')
	{
		write(2, "cd: only absolute or relative paths are allowed\n", 48);
		return (-1);
	}
	return (1);
}

int	builtin_cd(char **argv, char ***env)
{
	char	*oldpwd;
	char	*newpwd;
	int		ret;
	char	*path;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("cd: getcwd"), 1);
	if (handle_path(argv[1]) == -1)
		return (free(oldpwd), 1);
	if (!argv[1] || !*argv[1])
		path = getenv("HOME");
	else
		path = argv[1];
	if (chdir(path) == -1)
		return (write(2, "cd: ", 4), perror(path), free(oldpwd), 1);
	if (update_env(env, "OLDPWD", oldpwd) != 0)
		return (free(oldpwd), 1);
	free(oldpwd);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (perror("cd: getcwd"), 1);
	ret = update_env(env, "PWD", newpwd);
	return (free(newpwd), ret);
}
