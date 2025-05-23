/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:30:00 by abchaman          #+#    #+#             */
/*   Updated: 2025/05/22 15:13:48 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_struct(t_cmd *cmd)
{
	int	j;

	j = 0;
	cmd->av = malloc(sizeof(char *) * 1024);
	while (j < 1024)
	{
		cmd->av[j++] = NULL;
	}
	cmd->outfiles = NULL;
	cmd->infiles = NULL;
	cmd->append_flags = 0;
	cmd->heredoc = NULL;
	cmd->next = NULL;
	cmd->delimiter = NULL; // i jst added this, you need to handle the delimiter a broski
}
//khouya i had to handle this alone ma nigga do your work 
void	add_outfile(t_cmd *cmd, char *filename, int append)
{
	int		i;
	int		j;
	char	**new_outfiles;
	int		*new_flags;

	i = 0;
	while (cmd->outfiles && cmd->outfiles[i])
		i++;
	new_outfiles = malloc(sizeof(char *) * (i + 2));
	new_flags = malloc(sizeof(int) * (i + 2));
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
	free(cmd->outfiles);
	free(cmd->append_flags);
	cmd->outfiles = new_outfiles;
	cmd->append_flags = new_flags;
}

void	add_infile(t_cmd *cmd, char *filename)
{
	int		i;
	int		j;
	char	**new_infiles;

	i = 0;
	while (cmd->infiles && cmd->infiles[i])
		i++;
	new_infiles = malloc(sizeof(char *) * (i + 2));
	if (!new_infiles)
		exit(1);
	j = 0;
	while (j < i)
	{
		new_infiles[j] = cmd->infiles[j];
		j++;
	}
	new_infiles[i] = filename;
	new_infiles[i + 1] = NULL;
	free(cmd->infiles);
	cmd->infiles = new_infiles;
}

t_cmd    *start_of_parsing(t_token *tokens)
{
    int i;
    t_cmd *head;
    t_cmd *current;
    t_cmd *new_cmd;

    head = malloc(sizeof(t_cmd));
    if (!head)
        return NULL;
    init_struct(head);
    current = head;
    i = 0;
    while (tokens)
    {
        if (ft_strcmp(tokens->type, "PIPE") == 0)
        {
            new_cmd = malloc(sizeof(t_cmd));
            if (!new_cmd)
                return (NULL);
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
            if (tokens)
                current->heredoc = tokens->content;
        }
        tokens = tokens->next;
    }
    return (head);
}
