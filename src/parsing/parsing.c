/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:30:00 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/25 11:35:12 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_input(char *line, t_env *g_env, t_mem *gc)
{
	char	**splited;
	t_token	*token_list;
	t_cmd	*cmd;

	cmd = ft_malloc(gc, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->gc = gc;
	cmd->env = g_env;
	init_struct(cmd);
	if (handle_quotes_error(line))
		return (NULL);
	splited = smart_split(cmd, line);
	if (!splited)
		return (NULL);
	token_list = tokenize(cmd, splited);
	return (start_of_parsing(cmd, token_list));
}

void init_struct(t_cmd *cmd)
{
	int	j;

	j = 0;
	if (!cmd->gc)
		return ;
	if (!cmd->env)
		return ;
	cmd->av = ft_malloc(cmd->gc, sizeof(char *) * 1024);
	// !we need to alloc here carrefully
	while (j < 1024)
	{
		cmd->av[j++] = NULL;
	}
	cmd->outfiles = NULL;
	cmd->infiles = NULL;
	cmd->append_flags = NULL;
	cmd->heredoc = NULL;
	cmd->next = NULL;
	cmd->delimiter = NULL;
}

static void	add_outfile(t_cmd *cmd, char *filename, int append)
{
	int		i;
	int		j;
	char	**new_outfiles;
	int		*new_flags;

	i = 0;
	while (cmd->outfiles && cmd->outfiles[i])
		i++;
	new_outfiles = ft_malloc(cmd->gc, sizeof(char *) * (i + 2));
	new_flags = ft_malloc(cmd->gc, sizeof(int) * (i + 2));
	if (!new_outfiles || !new_flags)
		exit(1);
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
	cmd->append_flags = new_flags;
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
		ft_perror("Memory allocation failed\n");
		return;
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

t_cmd	*start_of_parsing(t_cmd *cmd, t_token *tokens)
{
	int		i;
	int		heredoc_counter;
	t_cmd	*head;
	t_cmd	*current;
	t_cmd	*new_cmd;

	head = ft_malloc(cmd->gc, sizeof(t_cmd));
	if (!head)
		return NULL;
	head->gc = cmd->gc;
	head->env = cmd->env;
	init_struct(head);
	current = head;
	i = 0;
	heredoc_counter = 0;
	while (tokens)
	{
		if (ft_strcmp(tokens->type, "PIPE") == 0)
		{
			new_cmd = ft_malloc(cmd->gc, sizeof(t_cmd));
			if (!new_cmd)
				return (NULL);
			new_cmd->gc = cmd->gc;
			new_cmd->env = cmd->env;
			init_struct(new_cmd);
			current->next = new_cmd;
			current = new_cmd;
			i = 0;
		}
		else if (ft_strcmp(tokens->type, "WORD") == 0)
				current->av[i++] = tokens->content;
		else if (ft_strcmp(tokens->type, "REDIRECTION_OUT") == 0)
		{
			tokens = tokens->next;
			if (tokens)
				add_outfile(current, tokens->content, 0);
		}
		else if (ft_strcmp(tokens->type, "APPEND") == 0)
		{
			tokens = tokens->next;
			if (tokens)
				add_outfile(current, tokens->content, 1);
		}
		else if (ft_strcmp(tokens->type, "REDIRECTION_IN") == 0)
		{
			tokens = tokens->next;
			if (tokens)
				add_infile(current, tokens->content);
		}
		else if (ft_strcmp(tokens->type, "HEREDOC") == 0)
		{
			tokens = tokens->next;
			if (tokens && ft_strcmp(tokens->type, "DELIMITER") == 0)
			{
				current->delimiter = tokens->content;
				current->heredoc = heredoc(current, cmd->gc, &heredoc_counter);
				if (!current->heredoc)
					return (ft_perror("Error: Failed to process heredoc\n"), NULL);
				current->delimiter = NULL;
			}
		}
		if (tokens)
			tokens = tokens->next;
	}
	return (head);
}
