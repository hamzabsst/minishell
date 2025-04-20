/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:35:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/20 11:51:47 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	copy_env(char ***env_ptr, char *new_var, int count)
{
	char **old = *env_ptr;
	char **new_env;
	int   j;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return (1);
	}
	j = 0;
	while (j < count)
	{
		new_env[j] = old[j];
		++j;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(old);
	*env_ptr = new_env;
	return (0);
}

int	update_env(char ***env_ptr, const char *key, const char *value)
{
	char   **env;
	char    *new_var;
	size_t   klen, vlen;
	int      i;

	if (!env_ptr || !(env = *env_ptr) || !key || !value)
		return (1);

	klen = ft_strlen(key);
	vlen = ft_strlen(value);

	new_var = malloc(klen + vlen + 2);
	if (!new_var)
		return (1);
	ft_strcpy(new_var, (char *)key);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, (char *)value);

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, klen) == 0 && env[i][klen] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return (0);
		}
		++i;
	}
	return (copy_env(env_ptr, new_var, i));
}

int	builtin_cd(char **argv, char ***env_ptr)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;
	int		ret;
	char	**env;
	int		i;

	oldpwd = getcwd(NULL, 0);
	if (!argv[1] || !*argv[1])
		path = getenv("HOME");
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = getenv("OLDPWD");
		ft_putendl_fd(path, 1);
	}
	else
		path = argv[1];

	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		free(oldpwd);
		return (1);
	}
	if (update_env(env_ptr, "OLDPWD", oldpwd) != 0)
	{
		free(oldpwd);
		return (1);
	}
	free(oldpwd);
	env = *env_ptr;
	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		++i;
	}

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("cd: getcwd");
		return (1);
	}
	ret = update_env(env_ptr, "PWD", newpwd);
	free(newpwd);
	return (ret);
}

