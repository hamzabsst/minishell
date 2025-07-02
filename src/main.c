/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 21:59:49 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/02 01:00:20 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		g_var = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static char *create_pwd_env(t_mem *gc)
{
	char	*cwd;
	char	*pwd_env;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		pwd_env = ft_malloc(gc, 6);
		if (pwd_env)
			ft_strlcpy(pwd_env, "PWD=/", 6);
		return (pwd_env);
	}
	pwd_env = ft_malloc(gc, ft_strlen(cwd) + 5);
	if (pwd_env)
	{
		ft_strlcpy(pwd_env, "PWD=", ft_strlen(cwd) + 5);
		ft_strlcat(pwd_env, cwd, ft_strlen(cwd) + 5);
	}
	free(cwd);
	return (pwd_env);
}

static t_env	*init_shell(char **env, t_mem *gc)
{
	t_env	*g_env;
	char	*mini_env[5];

	if (gc)
		gc->head = NULL;
	if (!env || !env[0])
	{
		mini_env[0] = create_pwd_env(gc);
		if (!mini_env[0])
			mini_env[0] = our_strdup(gc, "PWD=/");
		mini_env[1] = our_strdup(gc, "HOME=/tmp");
		mini_env[2] = our_strdup(gc, "SHELL=./minishell");
		mini_env[3] = our_strdup(gc, "PATH=/bin:/usr/bin:/usr/local/bin");
		mini_env[4] = NULL;
		g_env = dup_env(mini_env, gc);
	}
	else
		g_env = dup_env(env, gc);
	if (!g_env)
		return(our_perror("Failed to duplicate environment\n"), NULL);
	return (g_env);
}

static int	get_input(char **line, int exit_code, t_mem *gc)
{
	g_var = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	*line = readline(create_prompt(gc, exit_code));
	if (!*line)
		return (write(1, "exit\n", 5), 0);
	if (!**line)
		return (free(*line), 1);
	add_history(*line);
	return (2);
}

int	main(int ac, char **av, char **env)
{
	t_env	*g_env;
	t_mem	gc;
	t_cmd	*cmd;
	char	*line;
	int		exit_code;
	int		input;

	if (ac != 1)
		return ((void)av, our_perror("Invalid number of arguments\n"));
	g_env = init_shell(env, &gc);
	if (!g_env)
		return (1);
	exit_code = 0;
	while (1)
	{
		input = get_input(&line, exit_code, &gc);
		if (input == 0)
			break;
		if (input == 1)
			continue;
		signal(SIGINT, SIG_IGN);
		cmd = parse_input(line, g_env, &exit_code, &gc);
		free(line);
		if (cmd)
			exit_code = process_command(cmd);
	}
	ft_free_all(&gc);
	exit(exit_code);
}
