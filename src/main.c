/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/23 11:41:06 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd(char **args)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->av = args;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	free_cmd_list(t_cmd *cmd)
{
	while (cmd)
	{
		t_cmd *next = cmd->next;
		ft_free(cmd->av);
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->delimiter)
			free(cmd->delimiter);
		free(cmd);
		cmd = next;
	}
}

void handle_redirections(t_cmd *cmd)
{
    int i = 0;
    int j;

    while (cmd->av[i])
    {
        if (ft_strcmp(cmd->av[i], "<") == 0 && cmd->av[i + 1])
        {
            cmd->infile = ft_strdup(cmd->av[i + 1]);
            j = i;
            while (cmd->av[j + 2])
            {
                cmd->av[j] = cmd->av[j + 2];
                j++;
            }
            cmd->av[j] = NULL;
            continue;
        }
        else if (ft_strcmp(cmd->av[i], ">") == 0 && cmd->av[i + 1])
        {
            cmd->outfile = ft_strdup(cmd->av[i + 1]);
            cmd->append = 0;
            j = i;
            while (cmd->av[j + 2])
            {
                cmd->av[j] = cmd->av[j + 2];
                j++;
            }
            cmd->av[j] = NULL;
            continue;
        }
        else if (ft_strcmp(cmd->av[i], ">>") == 0 && cmd->av[i + 1])
        {
            cmd->outfile = ft_strdup(cmd->av[i + 1]);
            cmd->append = 1;
            j = i;
            while (cmd->av[j + 2])
            {
                cmd->av[j] = cmd->av[j + 2];
                j++;
            }
            cmd->av[j] = NULL;
            continue;
        }
        else if (ft_strcmp(cmd->av[i], "<<") == 0 && cmd->av[i + 1])
        {
            cmd->heredoc = 1;
            cmd->delimiter = ft_strdup(cmd->av[i + 1]);
            j = i;
            while (cmd->av[j + 2])
            {
                cmd->av[j] = cmd->av[j + 2];
                j++;
            }
            cmd->av[j] = NULL;
            continue;
        }
        i++;
    }
}

t_cmd	*mock_parse(char *input)
{
	t_cmd *head = NULL, *last = NULL;
	char **pipe_parts = ft_split(input, '|');
	if (!pipe_parts)
		return (NULL);

	for (int i = 0; pipe_parts[i]; i++)
	{
		char *trimmed = ft_strtrim(pipe_parts[i], " \t\n");
		if (!trimmed)
        {
            ft_free(pipe_parts);
            return (head);
        }
		if (*trimmed == '\0')
        {
            free(trimmed);
            continue;
		}
		char **args = ft_split(trimmed, ' ');
		free(trimmed);
		if (!args || !args[0])
        {
            if (args)
                ft_free(args);
			continue;
        }
		t_cmd *new_cmd = create_cmd(args);
		if (!new_cmd)
        {
            ft_free(args);
            ft_free(pipe_parts);
            free_cmd_list(head);
            return (NULL);
        }
        handle_redirections(new_cmd);
		if (!head)
			head = new_cmd;
		else
			last->next = new_cmd;
		last = new_cmd;
	}
	ft_free(pipe_parts);
	return (head);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**g_env = dup_env(env);
	int 	g_exit_status = 0;
	char prompt[100];

	if (ac != 1)
	{
		write(2, "Invalid number of arguments\n", 29);
		ft_free(g_env);
		exit(1);
	}
	(void)av;

	while (1)
	{
		snprintf(prompt, sizeof(prompt), "%sminishell%s%s$ ", RED, BLUE, RESET);
		line = readline(prompt);
		if (!line)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (*line)
			add_history(line);
		t_cmd *cmd = mock_parse(line);
		if (cmd)
		{
			if (builtin(cmd->av[0]))
				g_exit_status = exec_builtin(cmd, &g_env);
			else
				g_exit_status = exec_pipeline(cmd, g_env);
			free_cmd_list(cmd);
		}
		else if (*line)
		{
			printf("Parse error.\n");
			g_exit_status = 1;
		}
		free(line);
	}
	ft_free(g_env);
	return (g_exit_status);
}
