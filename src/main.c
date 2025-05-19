/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/19 11:14:25 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		sig = 0;

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**g_env;
	int		g_exit = 0;
	t_token	*token_list;
	t_cmd	*cmd;
	char	**splited;
	int		stdin_copy;
	int		stdout_copy;
	(void)av;

	if (ac != 1)
	{
		ft_perror("Invalid number of arguments\n");
		exit(1);
	}
	g_env = dup_env(env);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell$> ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (*line)
			add_history(line);
		splited = smart_split(line);
		if (!splited)
		{
			free(line);
			continue;
		}
		token_list = tokenize(splited);
		cmd = start_of_parsing(token_list);
		if (cmd)
		{
			if (builtin(cmd->av[0]) && !cmd->next)
			{
				stdin_copy = dup(STDIN_FILENO);
				stdout_copy = dup(STDOUT_FILENO);
				redirection(cmd);
				g_exit = exec_builtin(cmd, &g_env);
 				dup2(stdin_copy, STDIN_FILENO);
				dup2(stdout_copy, STDOUT_FILENO);
				close(stdin_copy);
				close(stdout_copy);
			}
			else
				g_exit = ft_exec(cmd, g_env);
		}
		else if (*line)
		{
			ft_perror("Parse error.\n");
			g_exit = 1;
		}
		free(cmd);
		ft_free(splited);
		free_token_list(token_list);
		free(line);
	}
	ft_free(g_env);
	exit(g_exit);
}

