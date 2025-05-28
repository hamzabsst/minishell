/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:35:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/28 10:44:50 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	copy_env(char ***env, char *new_var, int count, t_mem *collector)
{
	char	**old;
	char	**new_env;
	int		j;

	old = *env;
	new_env = ft_malloc(collector, sizeof(char *) * (count + 2));
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
	*env = new_env;
	return (0);
}

static char	*make_env_var(char *key, char *value, t_mem *collector)
{
	char	*var;
	size_t	len;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	var = ft_malloc(collector, len);
	if (!var)
		return (NULL);
	ft_strcpy(var, key);
	ft_strcat(var, "=");
	ft_strcat(var, value);
	return (var);
}

static int	handle_path(const char *arg)
{
	if (!arg || !*arg)
		return (0);
	if (arg[0] == '~' || arg[0] == '-')
	{
		ft_perror("cd: only absolute or relative paths are allowed\n");
		return (-1);
	}
	return (1);
}

int	update_env(char ***env_ptr, char *key, char *value, t_mem *collector)
{
	char	**env;
	char	*new_var;
	size_t	key_len;
	int		i;

	if (!env_ptr || !*env_ptr || !key || !value)
		return (1);
	env = *env_ptr;
	key_len = ft_strlen(key);
	new_var = make_env_var(key, value, collector);
	if (!new_var)
		return (1);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
		{
			env[i] = new_var;
			return (0);
		}
		i++;
	}
	return (copy_env(env_ptr, new_var, i, collector));
}

int	builtin_cd(char **argv, char ***env, t_mem *collector)
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
		path = ft_getenv(*env, "HOME");
	else
		path = argv[1];
	if (chdir(path) == -1)
		return (ft_perror("cd: "), perror(path), free(oldpwd), 1);
	if (update_env(env, "OLDPWD", oldpwd, collector) != 0)
		return (free(oldpwd), 1);
	free(oldpwd);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (perror("cd: getcwd"), 1);
	ret = update_env(env, "PWD", newpwd, collector);
	return (free(newpwd), ret);
}
