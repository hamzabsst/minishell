/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:35:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/20 10:19:31 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	copy_env(char ***env_ptr, char *new_var, int count)
{
	char	**new_env;
	int		j;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return (1);
	}
	j = 0;
	while (j < count)
	{
		new_env[j] = (*env_ptr)[j];
		j++;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(*env_ptr);
	*env_ptr = new_env;
	return (0);
}

int	update_env(char ***env_ptr, char *name, char *value)
{
	char	**env;
	char	*new_var;
	int		i;
	int		name_len;

	if (!env_ptr || !*env_ptr || !name || !value)
		return (1);
	env = *env_ptr;
	name_len = ft_strlen(name);
	new_var = malloc(name_len + ft_strlen(value) + 2);
	if (!new_var)
		return (1);
	ft_strcpy(new_var, name);
	strcat(new_var, "=");
	strcat(new_var, value);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return (0);
		}
		i++;
	}
	return (copy_env(env_ptr, new_var, i));
}

int	builtin_cd(char **argv, char ***env_ptr)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;
	int		ret;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("cd: getcwd");
		return (1);
	}
	if (!argv[1] || !*argv[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			free(oldpwd);
			return (1);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
			free(oldpwd);
			return (1);
		}
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
