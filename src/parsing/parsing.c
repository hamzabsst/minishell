/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:30:00 by abchaman          #+#    #+#             */
/*   Updated: 2025/07/01 15:20:31 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_outfile(t_cmd *cmd, char *filename, int append)
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
	{
		our_perror("Memory allocation failed\n");
		return;
	}
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
		our_perror("Memory allocation failed\n");
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

static int count_words_until_pipe(t_token *tokens)
{
	int count = 0;
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "WORD") == 0)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

static void init_struct(t_token *tokens, t_cmd *cmd, int exit_code, t_env *g_env, t_mem *gc)
{
	t_token	*temp;
	int		j;
	int		count;

	j = 0;
	temp = tokens;
	count = count_words_until_pipe(temp);
	cmd->gc = gc;
	cmd->env = g_env;
	cmd->av = ft_malloc(cmd->gc, sizeof(char *) * (count + 1));
	if (!cmd->av)
		return;
	while (j <= count)
		cmd->av[j++] = NULL;
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

static t_cmd	*start_of_parsing(t_token *tokens, t_env *g_env, int exit_code, t_mem *gc)
{
	int		i;
	int		heredoc_counter;
	t_cmd	*head;
	t_cmd	*current;
	t_cmd	*new_cmd;

	if (!tokens)
		return (NULL);
	head = ft_malloc(gc, sizeof(t_cmd));
	if (!head)
		return (NULL);
	init_struct(tokens, head, exit_code, g_env, gc);
	if (!head->av)
		return (NULL);
	current = head;
	i = 0;
	heredoc_counter = 0;
	while (tokens)
	{
		if (ft_strcmp(tokens->type, "PIPE") == 0)
		{
			new_cmd = ft_malloc(gc, sizeof(t_cmd));
			if (!new_cmd)
				return (NULL);
			init_struct(tokens->next, new_cmd, exit_code, g_env, gc);
			if (!new_cmd->av)
				return (NULL);
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
				current->heredoc = heredoc(current, &heredoc_counter);
				if (!current->heredoc)
					return (our_perror("Error: Failed to process heredoc\n"), NULL);
				current->delimiter = NULL;
			}
		}
		if (tokens)
			tokens = tokens->next;
	}
	return (head);
}

t_cmd	*parse_input(char *line, t_env *g_env, int *exit_code, t_mem *gc)
{
	char	**splited;
	t_token	*token_list;

	splited = mysplit(line, gc);
	if (!splited)
		return (NULL);
	token_list = tokenize(gc, splited);
	if (check_syntax_error(token_list, gc))
		return (*exit_code = 2, NULL);
	check_quotes(&token_list, gc);
	get_exit(&token_list, *exit_code, gc);
	return (start_of_parsing(token_list, g_env ,*exit_code, gc));
}
