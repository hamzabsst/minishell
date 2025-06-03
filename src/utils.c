/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:39:41 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/03 22:58:21 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_key(const char *arg)
{
	int	len;

	len = 0;
	while (arg[len] && arg[len] != '='
		&& !(arg[len] == '+' && arg[len + 1] == '='))
		len++;
	return (ft_substr(arg, 0, len));
}

int	update_env_append(char ***env, char *key, char *value, t_mem *collector)
{
	char	*old_value;
	char	*new_value;

	old_value = ft_getenv(*env, key);
	if (old_value)
		new_value = ft_strjoin(old_value, value);
	else
		new_value = ft_strdup(value);
	if (!new_value)
		return (1);
	update_env(env, key, new_value, collector);
	free(new_value);
	return (0);
}

char	**dup_env(char **env, t_mem *collector)
{
	int		len;
	int		i;
	char	**copy;

	len = 0;
	i = 0;
	while (env[len])
		len++;
	copy = ft_malloc(collector, sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	while(i < len)
	{
		copy[i] = ft_malloc(collector, strlen(env[i]) + 1);
		if (!copy[i])
			return (NULL);
		strcpy(copy[i], env[i]);
		i++;
	}
	copy[len] = NULL;
	return (copy);
}

char	*ft_getenv(char **env, const char *key)
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

int	ft_perror(char *msg)
{
	if (!msg)
		return (1);
	ft_putstr_fd(msg, 2);
	return (1);
}

char	*ft_strndup_mem(t_cmd *cmd, char *str, size_t len, char skip_single_q, char skip_double_q)
{
	size_t	i;
	char	*results;
	int		k;

	k = 0;
	results = ft_malloc(cmd->collector, len + 1);
	if (!results)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if(str[i] != skip_single_q && str[i] != skip_double_q)
			results[k++] = str[i];
		i++;
	}
	results[k] = '\0';
	return (results);
}

char	*ft_strdup_mem2(t_cmd *cmd, const char *s)
{
	size_t	i;
	size_t	size;
	char	*results;

	size = ft_strlen(s);
	results = ft_malloc(cmd->collector, (size + 1) * sizeof(char));
	if (!results)
		return (NULL);
	i = 0;
	while (s[i])
	{
		results[i] = s[i];
		i++;
	}
	results[i] = '\0';
	return (results);
}

char	*ft_strjoin_mem(t_mem *collector, char const *s1, char const *s2)
{
	char	*res;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	res = ft_malloc(collector , ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s1))
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < ft_strlen(s2))
	{
		res[i++] = s2[j++];
	}
	res[i] = '\0';
	return (res);
}

