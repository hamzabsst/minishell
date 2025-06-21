/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 11:55:21 by hbousset         ###   ########.fr       */
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

char	*find_key(const char *arg, t_mem *gc)
{
	int len;

	len = 0;
	while (arg[len] && arg[len] != '='
		&& !(arg[len] == '+' && arg[len + 1] == '='))
		len++;
	return (our_substr(arg, 0, len, gc));
}

char **env_to_array(t_cmd *cmd)
{
	t_expand	*current;
	char		**array;
	char		*temp;
	int			count;
	int			i;
	count = 0;
	current = cmd->env;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = ft_malloc(cmd->gc, sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	current = cmd->env;
	i = 0;
	while (current && i < count)
	{
		if (current->content && ft_strlen(current->content) > 0)
		{
			temp = ft_strjoin(current->var, "=");
			if (temp)
			{
				array[i] = ft_strjoin(temp, current->content);
				free(temp);
			}
		}
		else
			array[i] = our_strdup(cmd->gc, current->var);
		if (!array[i])
			return (NULL);
		current = current->next;
		i++;
	}
	array[i] = NULL;
	return (array);
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

static int	var_exists(t_expand *env, const char *key)
{
	t_expand *current;

	if (!env || !key)
		return (0);
	current = env;
	while (current)
	{
		if (current->var && ft_strcmp(current->var, key) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	print_export(t_cmd *cmd)
{
	char		**env_array;
	int			i;
	char		*equal;

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
			printf("declare -x %s=\"%s\"\n", env_array[i], equal + 1);
			*equal = '=';
		}
		else
			printf("declare -x %s\n", env_array[i]);
		i++;
	}
	return (0);
}

static void	process_av(t_cmd *cmd, char *arg)
{
	char	*key;
	char	*equal;

	key = find_key(arg, cmd->gc);
	if (!key)
		return;
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		if (equal > arg && *(equal - 1) == '+')
			update_env_append(cmd, key, equal + 1);
		else
			update_env(cmd, key, equal + 1);
	}
	else if (!var_exists(cmd->env, key))
		update_env(cmd, key, "");
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
		{
			ft_perror("export: `");
			ft_perror(cmd->av[i]);
			ft_perror("': not a valid identifier\n");
		}
		else
			process_av(cmd, cmd->av[i]);
		i++;
	}
	return (0);
}
