/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:54:09 by abchaman          #+#    #+#             */
/*   Updated: 2025/04/20 14:53:56 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *handle_doubleq(char *string)
{
    int i;
    int start;
    int len;
    char *token;

    token = NULL;
    len = 0;
    i = 0;
    while (string[i])
    {
        if(string[i] == '\"')
        {
            start = i + 1;
            i++;
            while (string[i] != '\0' && string[i] != '\"')
            {
                i++;
                len++;
            }
            if(string[i] == '\"')
                token = ft_substr(string, start, len);
            return (token);
        }
        i++;
    }
    return (token);
}

char *handle_singleq(char *string)
{
    int i;
    int start;
    int len;
    char *token;

    token = NULL;
    len = 0;
    i = 0;
    while (string[i])
    {
        if (string[i] == '\'')
        {
            start = i + 1;
            i++;
            while (string[i] != '\0' && string[i] != '\'')
            {
                i++;
                len++;
            }
            if(string[i] == '\'')
                token = ft_substr(string, start, len);
            return (token);
        }
        i++;
    }
    return (token);
}
void read_token(char **commands, t_cmd *cmd_list)
{
    int i;

    t_cmd *current = cmd_list;
    char *cmd_content = NULL;
    
    i = 0;
    while (current && commands[i])
    {
        if (ft_strchr(commands[i], '\''))
            cmd_content = handle_singleq(commands[i]);
        else if (ft_strchr(commands[i], '\"'))
            cmd_content = handle_doubleq(commands[i]);
        else
            cmd_content = ft_strdup(commands[i]);
        if (cmd_content)
        {
            current->argv = ft_split(cmd_content, ' ');
            free(cmd_content);
        }
        current = current->next;
        i++;
    }
}
