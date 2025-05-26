/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/25 13:44:11 by hbousset         ###   ########.fr       */
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

void debug_mem_count(t_mem *manager)
{
	t_mem_node *current = manager->head;
	int count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	printf("Memory manager has %d allocations\n", count);
}

int main(int ac, char **av, char **env)
{
	char	*line;
	char	**g_env;
	int	 g_exit = 0;
	t_token *token_list;
	t_cmd   *cmd;
	char	**splited;
	int	 stdin_copy;
	int	 stdout_copy;
	t_mem   mem_manager;
	(void)av;

	if (ac != 1)
	{
		ft_perror("Invalid number of arguments\n");
		exit(1);
	}

	g_env = dup_env(env);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	init_mem(&mem_manager);

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
		{
			free(line);
			continue;
		}
		cmd->mem_manager = &mem_manager;
		init_struct(cmd);
		splited = smart_split(cmd, line);
		if (!splited)
		{
			free(line);
			continue;
		}
		token_list = tokenize(cmd, splited);
		cmd = start_of_parsing(cmd, token_list);

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
			{
				g_sig = 1;
				g_exit = ft_exec(cmd, g_env);
				g_sig = 0;
			}
		}
		else if (*line)
		{
			ft_perror("Parse error.\n");
			g_exit = 1;
		}
		debug_mem_count(&mem_manager);
		ft_free_all(&mem_manager);
		init_mem(&mem_manager);
		free(line);
	}
	if (g_env)
	{
		int i = 0;
		while (g_env[i])
			free(g_env[i++]);
		free(g_env);
	}
	debug_mem_count(&mem_manager);
	ft_free_all(&mem_manager);
	exit(g_exit);
}
