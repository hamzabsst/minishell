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

int main(int ac, char **av)
{
    (void)av;
    (void)ac;
    t_token *list_of_tokens;
    t_cmd *parsing;
    
    int i = 0;
    char *line;
    char **splited;
    while (1)
    {
        line = readline("minishell$>");
        add_history(line);
        splited = smart_split(line);
        if (splited == NULL)
            return (1);
        list_of_tokens = tokenize(splited);
        print_tokens(list_of_tokens);
        parsing = start_of_parsing(list_of_tokens);
        // int j = 0;
        i = 0;
        while (parsing)
        {
            while (parsing->av[i])
            {
                printf("arg [%d] : %s\n", i, parsing->av[i]);
                i++;
            }
            if (parsing->infile)
                printf("Infile: %s\n", parsing->infile);
            if (parsing->outfile)
                printf("Outfile: %s\n", parsing->outfile);
            if (parsing->append)
                printf("Append: Yes\n");
            if (parsing->heredoc)
                printf("Heredoc: %s\n", parsing->heredoc);
            parsing = parsing->next;
        }
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
