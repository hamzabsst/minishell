/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:15:12 by abchaman          #+#    #+#             */
/*   Updated: 2025/04/22 10:56:07 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_struct(t_cmd *cmd)
{
	cmd->av = NULL;
	cmd->outfile = NULL;
	cmd->infile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->delimiter = NULL;
	cmd->next = NULL;
}

char	**split_pipe(char *line)
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

    while (1)
    {
        i = 0;
        line = readline("minishell$>");
        add_history(line);
        cmd_string = split_pipe(line);
        while (cmd_string[i])
        {
            t_cmd *new = malloc(sizeof(t_cmd));
            init_struct(new);
            new->av = smart_split(cmd_string[i]);
            ft_lstadd_backk(&cmd_list, new);
            i++;
        }
        int j = 0;
        i = 0;
        while (cmd_list && cmd_string[i])
        {
            i = 0;
            while (cmd_list->av[i])
            {
                printf("cmd list n %d Reading command %d: %s\n",j, i ,cmd_list->av[i]);
                i++;
            }
            cmd_list = cmd_list->next;
            j++;
        }
    }
}
