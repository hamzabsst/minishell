/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:43:25 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/15 15:25:36 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_quotes(char *str, t_expand *ctx)
{
	if (str[ctx->i] == '\'' && !ctx->in_dquote)
	{
		ctx->in_squote = !ctx->in_squote;
		ctx->result[ctx->j++] = str[ctx->i++];
	}
	else if (str[ctx->i] == '"' && !ctx->in_squote)
	{
		ctx->in_dquote = !ctx->in_dquote;
		ctx->result[ctx->j++] = str[ctx->i++];
	}
}

static void	process_character(char *str, t_expand *ctx, t_data *data)
{
	char	*expanded;
	int		i;

	if ((str[ctx->i] == '\'' && !ctx->in_dquote)
		|| (str[ctx->i] == '"' && !ctx->in_squote))
	{
		process_quotes(str, ctx);
	}
	else if (str[ctx->i] == '$' && !ctx->in_squote)
	{
		expanded = handle_dollar_sign(str, &ctx->i, data);
		if (expanded)
		{
			i = 0;
			while (expanded[i])
			{
				ctx->result[ctx->j] = expanded[i];
				ctx->j++;
				i++;
			}
		}
	}
	else
		ctx->result[ctx->j++] = str[ctx->i++];
}

static t_expand	*init_expand(char *str, t_data *data)
{
	t_expand	*ctx;

	ctx = ft_malloc(data->gc, sizeof(t_expand));
	if (!ctx)
		return (NULL);
	ctx->result_len = expand_len(str, data);
	ctx->result = ft_malloc(data->gc, ctx->result_len);
	if (!ctx->result)
		return (NULL);
	ctx->i = 0;
	ctx->j = 0;
	ctx->in_squote = 0;
	ctx->in_dquote = 0;
	return (ctx);
}

static char	*expand_vars(char *str, t_data *data)
{
	t_expand	*ctx;

	ctx = init_expand(str, data);
	if (!ctx)
		return (NULL);
	while (str[ctx->i])
		process_character(str, ctx, data);
	ctx->result[ctx->j] = '\0';
	return (ctx->result);
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
		if (current->type && !ft_strcmp(current->type, "WORD"))
		{
			if (!current->content)
				expanded = our_strdup(data->gc, "");
			else
				expanded = expand_vars(current->content, data);
			if (expanded)
				current->content = expanded;
		}
		current = current->next;
	}
}
