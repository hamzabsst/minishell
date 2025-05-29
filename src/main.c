/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/29 11:44:06 by hbousset         ###   ########.fr       */
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

char *get_short_pwd(t_mem *collector)
{
	char	*short_pwd;
	char	*cwd = getcwd(NULL, 0);
	char	*home = getenv("HOME");
	char	*last_slash;
	size_t	home_len;

	short_pwd = ft_malloc(collector, 256);
	if (!short_pwd)
		return (NULL);
	if (!cwd)
	{
		ft_strlcpy(short_pwd, "~", 256);
		return (short_pwd);
	}
	if (home && strstr(cwd, home) == cwd)
	{
		home_len = strlen(home);
		ft_strlcpy(short_pwd, "~", 256);
		ft_strlcat(short_pwd, cwd + home_len, 256);
	}
	else
		ft_strlcpy(short_pwd, cwd, 256);
	free(cwd);
	if (strlen(short_pwd) > 25)
	{
		last_slash = strrchr(short_pwd, '/');
		if (last_slash && last_slash != short_pwd)
		{
			ft_strlcpy(short_pwd, "...", 256);
			ft_strlcat(short_pwd, last_slash, 256);
		}
	}
	return (short_pwd);
}

char *create_prompt(t_mem *collector)
{
	char	*prompt;
	char	*pwd = get_short_pwd(collector);
	if (!pwd)
		return (NULL);
	prompt = ft_malloc(collector, 512);
	if (!prompt)
		return (NULL);

	prompt[0] = '\0';
	ft_strlcat(prompt, BRIGHT_CYAN, 512);
	ft_strlcat(prompt, "➜  ", 512);
	ft_strlcat(prompt, RESET, 512);
	ft_strlcat(prompt, BOLD_BLUE, 512);
	ft_strlcat(prompt, pwd, 512);
	ft_strlcat(prompt, RESET, 512);
	ft_strlcat(prompt, " ", 512);
	return (prompt);
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
	t_mem	collector;
	t_token	*token_list;
	t_cmd	*cmd;
	(void)av;

	if (ac != 1)
		exit(ft_perror("Invalid number of arguments\n"));
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	init_mem(&collector);
	g_env = dup_env(env, &collector);
	while (1)
	{
		line = readline(create_prompt(&collector));
		if (!line)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (*line)
			add_history(line);
		cmd = ft_malloc(&collector, sizeof(t_cmd));
		if (!cmd)
			continue;
		cmd->collector = &collector;
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
				redirection(cmd, &collector);
				g_exit = exec_builtin(cmd, &g_env, &collector);
				dup2(stdin_copy, STDIN_FILENO);
				dup2(stdout_copy, STDOUT_FILENO);
				close(stdin_copy);
				close(stdout_copy);
			}
			else
			{
				g_sig = 1;
				g_exit = ft_exec(cmd, g_env, &collector);
				g_sig = 0;
			}
		}
		else if (*line)
		{
			ft_perror("Parse error.\n");
			g_exit = 1;
		}
	}
	ft_free_all(&collector);
	exit(g_exit);
}
