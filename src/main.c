/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/23 11:10:44 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*parse_input(char *line, t_env *g_env, t_mem *gc)
{
	char	**splited;
	t_token	*token_list;
	t_cmd	*cmd;

	cmd = ft_malloc(gc, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->gc = gc;
	cmd->env = g_env;
	init_struct(cmd);
	if (handle_quotes_error(line))
		return (NULL);
	splited = smart_split(cmd, line);
	if (!splited)
		return (NULL);
	token_list = tokenize(cmd, splited);
	return (start_of_parsing(cmd, token_list));
}

static int get_input(char **line, t_mem *gc)
{
	*line = readline(create_prompt(gc));
	if (!*line)
		return (write(1, "exit\n", 5), 0);
	if (!**line)
		return (free(*line), 1);
	add_history(*line);
	return (2);
}

static void	restore_io(int in_copy, int out_copy)
{
	if (in_copy != -1)
		(dup2(in_copy, STDIN_FILENO), close(in_copy));
	if (out_copy != -1)
		(dup2(out_copy, STDOUT_FILENO), close(out_copy));
}

static int	backup_io(int *in_copy, int *out_copy)
{
	*in_copy = dup(STDIN_FILENO);
	*out_copy = dup(STDOUT_FILENO);
	if (*in_copy == -1 || *out_copy == -1)
	{
		if (*in_copy != -1)
			close(*in_copy);
		if (*out_copy != -1)
			close(*out_copy);
		return (-1);
	}
	return (0);
}

static void	handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		g_var = 1;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static t_env	*init_shell(char **env, t_mem *gc)
{
	t_env	*g_env;

	init_mem(gc);
	g_env = dup_env(env, gc);
	g_var = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (!g_env)
		exit(ft_perror("Failed to duplicate environment\n"));
	printf("\033[2J\033[H\n");
	return (g_env);
}

static int	process_command(t_cmd *cmd)
{
	int	in_copy;
	int	out_copy;
	int	exit_status;

	in_copy = -1;
	out_copy = -1;
	if (backup_io(&in_copy, &out_copy) == -1)
		return (ft_perror("Failed to backup stdio\n"));
	if (redirection(cmd) != 0)
		return (restore_io(in_copy, out_copy), 1);
	if (builtin(cmd->av[0]) && !cmd->next)
		(exit_status = exec_builtin(cmd) ,restore_io(in_copy, out_copy));
	else
	{
		g_var = 1;
		exit_status = ft_exec(cmd);
		if (g_var == 2)
		{
			g_var = 0;
			exit_status = 130;
		}
		else
			g_var = 0;
		restore_io(in_copy, out_copy);
	}
	return (exit_status);
}

int	main(int ac, char **av, char **env)
{
	t_env	*g_env;
	t_mem	gc;
	t_cmd	*cmd;
	char	*line;
	int		g_exit;
	int		input;

	if (ac != 1)
		return ((void)av, ft_perror("Invalid number of arguments\n"));
	g_env = init_shell(env, &gc);
	g_exit = 0;
	while (1)
	{
		g_var = 0;
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		input = get_input(&line, &gc);
		if (input == 0)
			break;
		if (input == 1)
			continue;
		signal(SIGINT, SIG_IGN);
		cmd = parse_input(line, g_env, &gc);
		free(line);
		if (cmd)
			g_exit = process_command(cmd);
		else
			g_exit = ft_perror("Parse error.\n");
	}
	ft_free_all(&gc);
	exit(g_exit);
}
