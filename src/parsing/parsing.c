/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:30:00 by abchaman          #+#    #+#             */
/*   Updated: 2025/07/02 23:55:18 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*parse_cmds(t_parse *data, int *exit_code)
{
	t_cmd	*head;

	if (!data->tokens)
		return (NULL);
	head = ft_malloc(data->gc, sizeof(t_cmd));
	if (!head)
		return (NULL);
	init_cmds(head, data, *exit_code);
	if (!head->av)
		return (NULL);
	return (process_tokens(head, data, exit_code));
}

static t_parse	*init_parsing(char *line, t_env *g_env, t_mem *gc)
{
	t_parse	*data;

	data = ft_malloc(gc, sizeof(t_parse));
	if (!data)
		return (NULL);
	data->env = g_env;
	data->line = line;
	data->gc = gc;
	data->splited = NULL;
	data->tokens = NULL;
	return (data);
}

t_cmd	*parse_input(char *line, t_env *g_env, int *exit_code, t_mem *gc)
{
	t_parse	*data;
	char	**tokens;
	t_data	*expand;
	data = init_parsing(line, g_env, gc);
	tokens = mysplit(data);
	if (!tokens)
		return (NULL);
	expand = ft_malloc(gc, sizeof(t_data));
	if (!expand)
		return (NULL);
	data->splited = tokens;
	data->tokens = tokenize(data);
	if (check_syntax_error(data))
		return (*exit_code = 2, NULL);
	expand->env = g_env;
	expand->exit_code = *exit_code;
	expand->gc = gc;
	// get_exit(&data->tokens, *exit_code, gc);
	expand_tokens(&data->tokens, expand);
	check_quotes(&data->tokens, gc);
	return (parse_cmds(data, exit_code));
}
