/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:15:12 by abchaman          #+#    #+#             */
/*   Updated: 2025/04/19 15:59:07 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void init_struct(t_command *cmd)
{
    cmd->argv = NULL;
    cmd->outfile = NULL;
    cmd->infile = NULL;
    cmd->append = 0;
    cmd->heredoc = 0;
    cmd->delimiter = NULL;
    cmd->next = NULL;
}

void    take_args(t_command *cmd, char **av)
{
    int count;
    int i;

    i = 0;
    count = 0;
    while (av[count])
        count++;
    cmd->argv = malloc(sizeof(char *) * (count + 1));
    if (!cmd->argv)
        return ;
    while (i < count)
    {
        cmd->argv[i] = ft_strdup(av[i]);
        if (!cmd->argv[i])
            return ;
        i++;
    }
    cmd->argv[count] = NULL;
}
int main(int ac, char **av)
{
    t_command cmd;
    init_struct(&cmd);
    if (ac > 1)
    {
        take_args(&cmd, &av[1]);
    }
}
