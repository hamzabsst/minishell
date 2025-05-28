/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/28 10:33:35 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_sig = 0;

void	handle_sigint(int sig)
{
	if(g_sig == 1)
		return ;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// Important Notes:
// You must pass your memory manager to ft_exec() and all related functions
// Make sure all your mallocs use ft_malloc(manager, size) instead of regular malloc()
// Any pointers from external functions (like strdup(), split(), etc.) should be added to the manager with ft_add_ptr()

int main(int ac, char **av, char **env)
{
	char	*line;
	char	**g_env;
	char	**splited;
	int		g_exit;
	int		stdin_copy;
	int		stdout_copy;
	t_mem	mem_manager;
	t_token	*token_list;
	t_cmd	*cmd;
	(void)av;

	if (ac != 1)
		exit(ft_perror("Invalid number of arguments\n"));
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	init_mem(&mem_manager);
	g_env = dup_env(env, &mem_manager);
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
		cmd = ft_malloc(&mem_manager, sizeof(t_cmd));
		if (!cmd)
			continue;
		cmd->mem_manager = &mem_manager;
		init_struct(cmd);
		splited = smart_split(cmd, line);
		if (!splited)
			continue;
		token_list = tokenize(cmd, splited);
		cmd = start_of_parsing(cmd, token_list);
		if (cmd)
		{
			if (builtin(cmd->av[0]) && !cmd->next)
			{
				stdin_copy = dup(STDIN_FILENO);
				stdout_copy = dup(STDOUT_FILENO);
				redirection(cmd, &mem_manager);
				g_exit = exec_builtin(cmd, &g_env, &mem_manager);
				dup2(stdin_copy, STDIN_FILENO);
				dup2(stdout_copy, STDOUT_FILENO);
				close(stdin_copy);
				close(stdout_copy);
			}
			else
			{
				g_sig = 1;
				g_exit = ft_exec(cmd, g_env, &mem_manager);
				g_sig = 0;
			}
		}
		else if (*line)
		{
			ft_perror("Parse error.\n");
			g_exit = 1;
		}
		//ft_free_all(&mem_manager);
		//init_mem(&mem_manager);
	}
		ft_free_all(&mem_manager);
		init_mem(&mem_manager);
		free(line);
	}
	if (g_env)
		ft_free(g_env);
	ft_free_all(&mem_manager);
	exit(g_exit);
}
