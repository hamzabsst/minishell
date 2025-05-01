/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/01 14:27:54 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**g_env;
	int		g_exit_status = 0;
	t_token	*token_list;
	t_cmd	*cmd;
	char	**splited;
	(void)av;

	if (ac != 1)
	{
		ft_perror("Invalid number of arguments\n");
		exit(1);
	}
	g_env = dup_env(env);
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
				g_exit_status = exec_builtin(cmd, &g_env);
			else
				g_exit_status = exec_pipeline(cmd, g_env);
		}
		else if (*line)
		{
			ft_perror("Parse error.\n");
			g_exit_status = 1;
		}
		free(cmd);
		ft_free(splited);
		free_token_list(token_list);
		free(line);
	}
	ft_free(g_env);
	return (g_exit_status);
}

