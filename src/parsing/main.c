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
	cmd->heredoc = NULL;
	cmd->next = NULL;
}

int main(int ac, char **av)
{
    (void)av;
    (void)ac;
    t_token *list_of_tokens;
    
    char *line;
    char **cmd_string;
    while (1)
    {
        line = readline("minishell$>");
        add_history(line);
        cmd_string = smart_split(line);
        list_of_tokens = tokenize(cmd_string);
        print_tokens(list_of_tokens);
        // int j = 0;
        // i = 0;
        // while (cmd_list && cmd_string[i])
        // {
        //     i = 0;
        //     while (cmd_list->av[i])
        //     {
        //         printf("cmd list n %d Reading command %d: %s\n",j, i ,cmd_list->av[i]);
        //         i++;
        //     }
        //     cmd_list = cmd_list->next;
        //     j++;
        // }
    }
}
