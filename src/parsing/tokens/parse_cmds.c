/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:25:09 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/03 15:25:53 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_til_pipe(t_token *tokens)
{
	int	count;

	count = 0;
	if (tokens && ft_strcmp(tokens->type, "PIPE") == 0)
		tokens = tokens->next;
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "WORD") == 0)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

void	init_cmds(t_cmd *cmd, t_parse *data, int exit_code)
{
	t_token	*temp;
	int		j;
	int		count;

	j = 0;
	temp = data->tokens;
	count = count_til_pipe(temp);
	cmd->av = ft_malloc(data->gc, sizeof(char *) * (count + 1));
	if (!cmd->av)
		return ;
	while (j < count + 1)
		cmd->av[j++] = NULL;
	cmd->gc = data->gc;
	cmd->env = data->env;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->append_flags = 0;
	cmd->heredoc = NULL;
	cmd->delimiter = NULL;
	cmd->heredoc_processed = 0;
	cmd->forked = 0;
	cmd->exit_code = exit_code;
	cmd->in_copy = -1;
	cmd->out_copy = -1;
	cmd->next = NULL;
}

static t_cmd	*create_cmd(t_parse *data, int exit_code)
{
	t_cmd	*new_cmd;

	new_cmd = ft_malloc(data->gc, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	init_cmds(new_cmd, data, exit_code);
	if (!new_cmd->av)
		return (NULL);
	return (new_cmd);
}

static int	parse_token(t_cmd **current, t_parse *data, int *i, int *exit_code)
{
	int	count;

	count = 0;
	if (ft_strcmp(data->tokens->type, "PIPE") == 0)
	{
		(*current)->next = create_cmd(data, *exit_code);
		if (!(*current)->next)
			return (-1);
		*current = (*current)->next;
		*i = 0;
	}
	else if (ft_strcmp(data->tokens->type, "WORD") == 0)
	{
		(*current)->av[(*i)++] = data->tokens->content;
	}
	else if (ft_strcmp(data->tokens->type, "HEREDOC") == 0)
	{
		if (parse_heredoc(&data->tokens, *current, &count, exit_code))
			return (-1);
	}
	else
		parse_redir(&data->tokens, *current);
	return (0);
}

t_cmd	*process_tokens(t_cmd *head, t_parse *data, int *exit_code)
{
	t_cmd	*current;
	int		i;

	current = head;
	i = 0;
	while (data->tokens)
	{
		if (parse_token(&current, data, &i, exit_code) == -1)
			return (NULL);
		if (data->tokens)
			data->tokens = data->tokens->next;
	}
	return (head);
}
