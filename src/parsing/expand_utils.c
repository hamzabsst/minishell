/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:00:31 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/15 15:15:14 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_var_len(char *str, int start)
{
	int	len;

	len = 0;
	while (str[start + len] && (ft_isalnum(str[start + len])
			|| str[start + len] == '_'))
		len++;
	return (len);
}

char	*get_var_name(char *str, int start, int len, t_mem *gc)
{
	char	*var_name;
	int		i;

	var_name = ft_malloc(gc, len + 1);
	if (!var_name)
		return (NULL);
	i = 0;
	while (i < len)
	{
		var_name[i] = str[start + i];
		i++;
	}
	var_name[i] = '\0';
	return (var_name);
}

static char	*handle_exit_code(int *i, t_data *data)
{
	char	*exit_str;

	(*i)++;
	exit_str = ft_itoa(data->exit_code);
	if (!exit_str)
		return (NULL);
	ft_add_ptr(data->gc, exit_str);
	return (our_strdup(data->gc, exit_str));
}

static char	*handle_variable(char *str, int *i, t_data *data)
{
	char	*var_name;
	char	*var_value;
	char	*result;
	int		var_len;

	var_len = get_var_len(str, *i);
	if (var_len > 0)
	{
		var_name = get_var_name(str, *i, var_len, data->gc);
		if (var_name)
		{
			var_value = ft_getenv(data->env, var_name);
			result = our_strdup(data->gc, var_value);
			*i += var_len;
			return (result);
		}
	}
	return (our_strdup(data->gc, "$"));
}

char	*handle_dollar_sign(char *str, int *i, t_data *data)
{
	(*i)++;
	if (str[*i] == '$')
	{
		(*i)++;
		return (our_strdup(data->gc, "$"));
	}
	else if (str[*i] == '?')
		return (handle_exit_code(i, data));
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
		return (handle_variable(str, i, data));
	return (our_strdup(data->gc, "$"));
}
