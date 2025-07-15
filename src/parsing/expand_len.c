/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:51:37 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/15 15:22:01 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exitcode_len(t_data *data)
{
	char	*exit_str;
	int		len;

	exit_str = ft_itoa(data->exit_code);
	if (!exit_str)
		return (0);
	len = ft_strlen(exit_str);
	free(exit_str);
	return (len);
}

static int	var_len(char *str, int i, t_data *data)
{
	int		var_len;
	char	*var_name;
	char	*var_value;

	var_len = get_var_len(str, i);
	if (var_len <= 0)
		return (1);
	var_name = get_var_name(str, i, var_len, data->gc);
	if (!var_name)
		return (0);
	var_value = ft_getenv(data->env, var_name);
	return (ft_strlen(var_value));
}

static int	handle_dollar_expand(char *str, int *i, t_data *data)
{
	int	length;

	if (str[++(*i)] == '$')
	{
		(*i)++;
		return (1);
	}
	else if (str[*i] == '?')
	{
		(*i)++;
		return (exitcode_len(data));
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		length = var_len(str, *i, data);
		if (length > 1)
		{
			if (get_var_len(str, *i) > 0)
				*i += get_var_len(str, *i);
			return (length);
		}
		else
			return (1);
	}
	return (1);
}

static void	handle_quotes_expand(char c, int *in_squote, int *in_dquote, int *i)
{
	if (c == '\'' && !(*in_dquote))
		*in_squote = !(*in_squote);
	else if (c == '"' && !(*in_squote))
		*in_dquote = !(*in_dquote);
	(*i)++;
}

int	expand_len(char *str, t_data *data)
{
	int	total_len;
	int	i;
	int	in_squote;
	int	in_dquote;

	total_len = 0;
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && !in_dquote) || (str[i] == '"' && !in_squote))
		{
			handle_quotes_expand(str[i], &in_squote, &in_dquote, &i);
			total_len++;
		}
		else if (str[i] == '$' && !in_squote)
			total_len += handle_dollar_expand(str, &i, data);
		else
		{
			total_len++;
			i++;
		}
	}
	return (total_len + 1);
}
