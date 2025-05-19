/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:03:59 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/01 14:03:59 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_tokens(t_token *tokens)
{
    int i = 0;
    while (tokens)
    {
        printf("Token[%d]: %s Type: %s\n", i, tokens->content, tokens->type);
        tokens = tokens->next;
        i++;
    }
}
void add_token_back(t_token **head, t_token *new)
{
    t_token *tmp;
    if (!*head)
    {
        *head = new;
        return ;
    }
    tmp = *head;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = new;
}

t_token *allocate_token(char *content, char *type)
{
    t_token *new;

    new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->content = ft_strdup(content);
    new->type = ft_strdup(type);
    new->next = NULL;
    return (new);
}
t_token *tokenize(char **tokens)
{
	t_token	*head;
	int		i;
	char	*type;
    t_token *new;
    i = 0;
    head = NULL;
    while (tokens[i])
    {
        type = "WORD";
        if (ft_strcmp(tokens[i], "|") == 0)
            type = "PIPE";
        else if (ft_strcmp(tokens[i], ">") == 0)
            type = "REDIRECTION_OUT";
        else if (ft_strcmp(tokens[i], "<") == 0)
            type = "REDIRECTION_IN";
        else if(ft_strcmp(tokens[i], ">>") == 0)
            type = "APPEND";
        else if (ft_strcmp(tokens[i], "<<") == 0)
            type = "HEREDOC";
        new = allocate_token(tokens[i], type);
        add_token_back(&head, new);
        i++;
    }
    return (head);
}

void	free_token_list(t_token *token)
{
	t_token *next;

	while (token)
	{
		next = token->next;
		free(token->content);
		free(token->type);
		free(token);
		token = next;
	}
}

