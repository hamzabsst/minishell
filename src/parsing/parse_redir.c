/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:32:23 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/07 15:26:32 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_outfile(t_cmd *cmd, char *filename, int append)
{
	char	**new_outfiles;
	bool	*new_flags;
	int		i;
	int		j;

	i = 0;
	while (cmd->outfiles && cmd->outfiles[i])
		i++;
	new_outfiles = ft_malloc(cmd->gc, sizeof(char *) * (i + 2));
	new_flags = ft_malloc(cmd->gc, sizeof(int) * (i + 2));
	if (!new_outfiles || !new_flags)
		return (our_error("Memory allocation failed\n"));
	j = 0;
	while (j < i)
	{
		new_outfiles[j] = cmd->outfiles[j];
		new_flags[j] = cmd->append_flags[j];
		j++;
	}
	new_outfiles[i] = filename;
	new_flags[i] = append;
	new_outfiles[i + 1] = NULL;
	new_flags[i + 1] = 0;
	cmd->outfiles = new_outfiles;
	return (cmd->append_flags = new_flags, 0);
}

static void	add_infile(t_cmd *cmd, char *filename)
{
	int		i;
	int		j;
	char	**infiles;

	i = 0;
	while (cmd->infiles && cmd->infiles[i])
		i++;
	infiles = ft_malloc(cmd->gc, sizeof(char *) * (i + 2));
	if (!infiles)
	{
		our_error("Memory allocation failed\n");
		return ;
	}
	j = 0;
	while (j < i)
	{
		infiles[j] = cmd->infiles[j];
		j++;
	}
	infiles[i] = filename;
	infiles[i + 1] = NULL;
	cmd->infiles = infiles;
}

int	parse_redir(t_token **tokens, t_cmd *current)
{
	if (!ft_strcmp((*tokens)->type, "REDIRECTION_OUT"))
	{
		*tokens = (*tokens)->next;
		if (*tokens)
			add_outfile(current, (*tokens)->content, 0);
	}
	else if (!ft_strcmp((*tokens)->type, "APPEND"))
	{
		*tokens = (*tokens)->next;
		if (*tokens)
			add_outfile(current, (*tokens)->content, 1);
	}
	else if (!ft_strcmp((*tokens)->type, "REDIRECTION_IN"))
	{
		current->heredoc = NULL;
		current->delimiter = NULL;
		*tokens = (*tokens)->next;
		if (*tokens)
			add_infile(current, (*tokens)->content);
	}
	else
		return (0);
	return (1);
}

int	parse_heredoc(t_token **tokens, t_cmd *current, int *count, int *exit_code)
{
	*tokens = (*tokens)->next;
	if (*tokens && !ft_strcmp((*tokens)->type, "DELIMITER"))
	{
		current->infiles = NULL;
		current->delimiter = (*tokens)->content;
		current->heredoc = heredoc(current, count, exit_code);
		if (!current->heredoc)
		{
			if (*exit_code == 130)
				return (1);
			return (our_error("Error: Failed to process heredoc\n"), -1);
		}
		current->delimiter = NULL;
	}
	return (0);
}
