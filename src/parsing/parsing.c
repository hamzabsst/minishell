/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:30:00 by abchaman          #+#    #+#             */
/*   Updated: 2025/04/22 10:56:17 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_struct(t_cmd *cmd)
{
    int j;
    j = 0;

    cmd->av = malloc(sizeof(char *) * 1024);
    while (j < 1024)
    {
        cmd->av[j++] = NULL;
    }
	cmd->outfile = NULL;
	cmd->infile = NULL;
	cmd->append = 0;
	cmd->heredoc = NULL;
	cmd->next = NULL;
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
                current->outfile = tokens->content;
        }    
        else if (ft_strcmp(tokens->type, "REDIRECTION_IN") == 0)
        {
            tokens = tokens->next;
            if (tokens)
                current->infile = tokens->content;
        }
        else if (ft_strcmp(tokens->type, "APPEND") == 0)
        {
            tokens = tokens->next;
            if (tokens)
            {
                current->outfile = tokens->content;
                current->append = 1;
            }
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
