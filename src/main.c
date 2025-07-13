/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/13 19:29:07 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*init_shell(char **env, t_mem *gc)
{
	t_env	*g_env;
	char	*mini_env[5];

	if (gc)
		gc->head = NULL;
	if (!env || !env[0])
	{
		mini_env[0] = our_strdup(gc, "PWD=/home/");
		mini_env[1] = our_strdup(gc, "HOME=/tmp");
		mini_env[2] = our_strdup(gc, "SHELL=./minishell");
		mini_env[3] = our_strdup(gc, "PATH=/bin:/usr/bin:/usr/local/bin");
		mini_env[4] = NULL;
		g_env = dup_env(mini_env, gc);
	}
	else
		g_env = dup_env(env, gc);
	if (!g_env)
		return (our_error("Failed to duplicate environment\n"), NULL);
	return (g_env);
}

static void	handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		g_var = 2;
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static int	get_input(char **line, int exit_code, t_mem *gc)
{
	g_var = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	rl_outstream = stderr;
	*line = readline(create_prompt(gc, exit_code));
	if (!*line)
		return (write(1, "exit\n", 5), 0);
	if (!**line)
		return (free(*line), 1);
	add_history(*line);
	return (2);
}

static void	minishell(t_mem *gc, t_env *g_env, int *exit_code)
{
	t_cmd	*cmd;
	char	*line;
	int		input;

	while (1)
	{
		input = get_input(&line, *exit_code, gc);
		if (input == 0)
			break ;
		if (input == 1)
			continue ;
		if (g_var == 2)
		{
			*exit_code = 130;
			g_var = 0;
		}
		signal(SIGINT, SIG_IGN);
		cmd = parse_input(line, g_env, exit_code, gc);
		free(line);
		if (cmd)
			*exit_code = process_command(cmd);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*g_env;
	t_mem	gc;
	int		exit_code;

	if (ac != 1)
		return ((void)av, our_error("Invalid number of arguments\n"));
	g_env = init_shell(env, &gc);
	if (!g_env)
		return (1);
	exit_code = 0;
	minishell(&gc, g_env, &exit_code);
	ft_free_all(&gc);
	exit(exit_code);
}
