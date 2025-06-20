/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/20 11:04:14 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	if(g_sig == 1)
	{
		write(1, "\n", 1);
		return ;
	}
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	restore_io(int stdin_copy, int stdout_copy)
{
	if (stdin_copy != -1)
		(dup2(stdin_copy, STDIN_FILENO), close(stdin_copy));
	if (stdout_copy != -1)
		(dup2(stdout_copy, STDOUT_FILENO), close(stdout_copy));
}

static int	backup_io(int *stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	*stdout_copy = dup(STDOUT_FILENO);
	if (*stdin_copy == -1 || *stdout_copy == -1)
	{
		if (*stdin_copy != -1)
			close(*stdin_copy);
		if (*stdout_copy != -1)
			close(*stdout_copy);
		return (-1);
	}
	return (0);
}

static int	process_command(t_cmd *cmd, char ***g_env, t_mem *collector)
{
	int	stdin_copy;
	int	stdout_copy;
	int	exit_status;

	stdin_copy = -1;
	stdout_copy = -1;
	if (backup_io(&stdin_copy, &stdout_copy) == -1)
		return (ft_perror("Failed to backup stdio\n"));
	if (redirection(cmd) != 0)
	{
		restore_io(stdin_copy, stdout_copy);
		return (1);
	}
	if (redirection(cmd) != 0)
		return (restore_io(stdin_copy, stdout_copy), 1);
	if (builtin(cmd->av[0]) && !cmd->next)
	{
		exit_status = exec_builtin(cmd, g_env, collector);
		restore_io(stdin_copy, stdout_copy);
	}
	else
	{
		g_sig = 1;
		exit_status = ft_exec(cmd, *g_env, collector);
		g_sig = 0;
		restore_io(stdin_copy, stdout_copy);
	}
	return (exit_status);
}

static char	**init_shell(int ac, char **env, t_mem *collector)
{
	char	**g_env;

	if (ac != 1)
		exit(ft_perror("Invalid number of arguments\n"));
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	init_mem(collector);
	g_env = dup_env(env, collector);
	if (!g_env)
		exit(ft_perror("Failed to duplicate environment\n"));
	return (g_env);
}

static t_cmd	*parse_input(char *line, t_mem *collector)
{
	char	**splited;
	t_token	*token_list;
	t_cmd	*cmd;

	cmd = ft_malloc(collector, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->collector = collector;
	init_struct(cmd);
	if (handle_quotes_error(line))
		return (NULL);
	splited = smart_split(cmd, line);
	if (!splited)
		return (NULL);
	token_list = tokenize(cmd, splited);
	return (start_of_parsing(cmd, token_list));
}

static int get_input(char **line, t_mem *collector)
{
	*line = readline(create_prompt(collector));
	if (!*line)
		return (write(1, "exit\n", 5), 0);
	if (!**line)
		return (free(*line), 1);
	add_history(*line);
	return (2);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**g_env;
	int		g_exit;
	t_mem	collector;
	t_cmd	*cmd;
	int		input;
	(void)av;

	g_env = init_shell(ac, env, &collector);
	g_exit = 0;
	g_sig = 0;
	while (1)
	{
		input = get_input(&line, &collector);
		if (input == 0)
			break;
		if (input == 1)
			continue;
		cmd = parse_input(line, &collector);
		free(line);
		if (cmd)
			g_exit = process_command(cmd, &g_env, &collector);
		else
			g_exit = ft_perror("Parse error.\n");
	}
	ft_free_all(&collector);
	exit(g_exit);
}
