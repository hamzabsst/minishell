#include "minishell.h"

static char	*get_env_value(char *var_name, t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->var, var_name) == 0)
		{
			if (current->content)
				return (current->content);
			return ("");
		}
		current = current->next;
	}
	return ("");
}

static int	get_var_len(char *str, int start)
{
	int	len;

	len = 0;
	while (str[start + len] && (ft_isalnum(str[start + len]) 
			|| str[start + len] == '_'))
		len++;
	return (len);
}

static char	*get_var_name(char *str, int start, int len, t_mem *gc)
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

static char	*handle_double_dollar(int *i, t_mem *gc)
{
	char	*result;

	(*i)++;
	result = ft_malloc(gc, 2);
	if (!result)
		return (NULL);
	result[0] = '$';
	result[1] = '\0';
	return (result);
}

static char	*handle_exit_code(int *i, int exit_code, t_mem *gc)
{
	char	*exit_str;

	(*i)++;
	exit_str = ft_itoa(exit_code);
	if (!exit_str)
		return (NULL);
	ft_add_ptr(gc, exit_str);
	return (our_strdup(gc, exit_str));
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
			var_value = get_env_value(var_name, data->env);
			result = our_strdup(data->gc, var_value);
			*i += var_len;
			return (result);
		}
	}
	return (our_strdup(data->gc, "$"));
}

static char	*handle_dollar_sign(char *str, int *i, t_data *data)
{
	(*i)++;
	if (str[*i] == '$')
		return (handle_double_dollar(i, data->gc));
	else if (str[*i] == '?')
		return (handle_exit_code(i, data->exit_code, data->gc));
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
		return (handle_variable(str, i, data));
	return (our_strdup(data->gc, "$"));
}

static void	copy_expanded_to_result(char *result, int *j, char *expanded)
{
	int	k;

	k = 0;
	while (expanded[k])
	{
		result[*j] = expanded[k];
		(*j)++;
		k++;
	}
}

static int	calculate_expanded_length(char *str, t_data *data)
{
	int		total_len;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;
	int		var_len;
	char	*var_name;
	char	*var_value;
	char	*exit_str;

	total_len = 0;
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			total_len++;
			i++;
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			total_len++;
			i++;
		}
		else if (str[i] == '$' && !in_single_quotes)
		{
			i++;
			if (str[i] == '$')
			{
				total_len++;
				i++;
			}
			else if (str[i] == '?')
			{
				exit_str = ft_itoa(data->exit_code);
				if (exit_str)
				{
					total_len += ft_strlen(exit_str);
					free(exit_str);
				}
				i++;
			}
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				var_len = get_var_len(str, i);
				if (var_len > 0)
				{
					var_name = get_var_name(str, i, var_len, data->gc);
					if (var_name)
					{
						var_value = get_env_value(var_name, data->env);
						total_len += ft_strlen(var_value);
					}
					i += var_len;
				}
				else
				{
					total_len++;
				}
			}
			else
			{
				total_len++;
			}
		}
		else
		{
			total_len++;
			i++;
		}
	}
	return (total_len + 1);
}

static char	*expand_vars(char *str, t_data *data)
{
	char	*result;
	char	*expanded;
	int		i;
	int		j;
	int		in_single_quotes;
	int		in_double_quotes;
	int		result_len;

	result_len = calculate_expanded_length(str, data);
	result = ft_malloc(data->gc, result_len);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			result[j++] = str[i++];
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			result[j++] = str[i++];
		}
		else if (str[i] == '$' && !in_single_quotes)
		{
			expanded = handle_dollar_sign(str, &i, data);
			if (expanded)
			{
				copy_expanded_to_result(result, &j, expanded);
			}
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*expand_string(char *str, t_data *data)
{
	char	*result;

	if (!str || !*str)
		return (our_strdup(data->gc, ""));
	result = expand_vars(str, data);
	return (result);
}

void	expand_tokens(t_token **tokens, t_data *data)
{
	t_token	*current;
	char	*expanded;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current)
	{
		if (current->type && ft_strcmp(current->type, "WORD") == 0)
		{
			expanded = expand_string(current->content, data);
			if (expanded)
			{
				current->content = expanded;
			}
		}
		current = current->next;
	}
}