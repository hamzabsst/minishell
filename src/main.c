/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/03 22:42:37 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Important Notes:
//found a case:
// ➜  ~/minishell echo ""hamza""< > >>""pwd""
// it create a file called >>
//however bash
//echo ""hamza""< > >>""pwd""
//bash: syntax error near unexpected token `>'

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
	exit_status = 0;

	if (backup_io(&stdin_copy, &stdout_copy) == -1)
		return (ft_perror("Failed to backup stdio\n"));

	if (redirection(cmd, collector) != 0)
		return (restore_io(stdin_copy, stdout_copy), 1);
	if (builtin(cmd->av[0]) && !cmd->next)
		exit_status = exec_builtin(cmd, g_env, collector);
	else
	{
		restore_io(stdin_copy, stdout_copy);
		stdin_copy = -1;
		stdout_copy = -1;
		g_sig = 1;
		exit_status = ft_exec(cmd, *g_env, collector);
		g_sig = 0;
		return (exit_status);
	}
	restore_io(stdin_copy, stdout_copy);
	return (exit_status);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**g_env;
	char	**splited;
	int		g_exit;
	t_mem	collector;
	t_token	*token_list;
	t_cmd	*cmd;
	(void)av;

	g_exit = 0;
	if (ac != 1)
		exit(ft_perror("Invalid number of arguments\n"));
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	init_mem(&collector);
	g_env = dup_env(env, &collector);
	if (!g_env)
		exit(ft_perror("Failed to duplicate environment\n"));
	while (1)
	{
		line = readline(create_prompt(&collector));
		if (!line)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (!*line)
		{
			free(line);
			continue;
		}
		add_history(line);
		cmd = ft_malloc(&collector, sizeof(t_cmd));
		if (!cmd)
		{
			free(line);
			continue;
		}
		cmd->collector = &collector;
		init_struct(cmd);
		if(handle_quotes_error(line))
		{
			free(line);
			continue;
		}
		splited = smart_split(cmd, line);
		free(line);
		if (!splited)
			continue;
		token_list = tokenize(cmd, splited);
		cmd = start_of_parsing(cmd, token_list);
		if (cmd)
			g_exit = process_command(cmd, &g_env, &collector);
		else
			g_exit = ft_perror("Parse error.\n");
	}
	ft_free_all(&collector);
	exit(g_exit);
}
