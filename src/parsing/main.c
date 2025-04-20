/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:15:12 by abchaman          #+#    #+#             */
/*   Updated: 2025/04/20 14:56:41 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void init_struct(t_cmd *cmd)
{
    cmd->argv = NULL;
    cmd->outfile = NULL;
    cmd->infile = NULL;
    cmd->append = 0;
    cmd->heredoc = 0;
    cmd->delimiter = NULL;
    cmd->next = NULL;
}

char **split_pipe(char *line)
{
    char **splitone;
    if (!line)
        return (NULL);
    splitone = ft_split(line, '|');
    if (!splitone)
        return (NULL);
    return (splitone);
}

int main(int ac, char **av)
{
    (void)av;
    (void)ac;
    t_cmd *cmd_list = NULL;
    char *line;
    char **cmd_string;
    int i;

    i = 0;
    line = readline("minishell$>");
    cmd_string = split_pipe(line);
    while (cmd_string[i])
    {
        t_cmd *new = malloc(sizeof(t_cmd));
        init_struct(new);
        // printf("%s\n", cmd_string[i]);
        ft_lstadd_backk(&cmd_list, new);
        i++;
    }
    i = 0;
    read_token(cmd_string, cmd_list);
    while (cmd_list && cmd_string[i])
    {
        i = 0;
        while (cmd_list->argv[i])
        {
            printf("Reading command: %s\n", cmd_list->argv[i]);
            i++;
        }
        cmd_list = cmd_list->next;
    }
}
