/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:34:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/01 13:53:32 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_perror(const char *prefix, const char *token, const char *suffix, t_mem *gc)
{
	char	*complete_msg;
	size_t	total_len;

	if (!prefix)
		return (1);
	total_len = ft_strlen(prefix);
	if (token)
		total_len += ft_strlen(token);
	if (suffix)
		total_len += ft_strlen(suffix);
	complete_msg = ft_malloc(gc, total_len + 1);
	if (!complete_msg)
		return (1);
	ft_strlcpy(complete_msg, prefix, total_len + 1);
	if (token)
		ft_strlcat(complete_msg, token, total_len + 1);
	if (suffix)
		ft_strlcat(complete_msg, suffix, total_len + 1);
	ft_putstr_fd(complete_msg, 2);
	return (1);
}

static char	*get_short_pwd(t_mem *gc)
{
	char	*short_pwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	short_pwd = ft_malloc(gc, 256);
	if (!cwd || !short_pwd)
		return (free(cwd), ft_strlcpy(short_pwd, "~", 256), short_pwd);
	if (getenv("HOME") && ft_strnstr(cwd, getenv("HOME"), ft_strlen(cwd)) == cwd)
	{
		ft_strlcpy(short_pwd, "~", 256);
		ft_strlcat(short_pwd, cwd + ft_strlen(getenv("HOME")), 256);
	}
	else
		ft_strlcpy(short_pwd, cwd, 256);
	free(cwd);
	if (ft_strlen(short_pwd) > 28 && ft_strrchr(short_pwd, '/')
		&& ft_strrchr(short_pwd, '/') != short_pwd)
	{
		ft_strlcpy(short_pwd, "...", 256);
		ft_strlcat(short_pwd, ft_strrchr(short_pwd, '/'), 256);
	}
	return (short_pwd);
}

static char	*get_username(t_mem *gc)
{
	char	*username;
	const char	*user_env;

	user_env = getenv("USER");
	if (!user_env)
		user_env = getenv("LOGNAME");
	if (!user_env)
		user_env = "user";
	username = ft_malloc(gc, ft_strlen(user_env) + 1);
	if (!username)
		return (NULL);
	ft_strlcpy(username, user_env, ft_strlen(user_env) + 1);
	return (username);
}

static void	add_color(char *prompt, const char *color, size_t size)
{
	char	temp[64];

	ft_strlcpy(temp, "\001", sizeof(temp));
	ft_strlcat(temp, color, sizeof(temp));
	ft_strlcat(temp, "\002", sizeof(temp));
	ft_strlcat(prompt, temp, size);
}

char	*create_prompt(t_mem *gc, int exit_code)
{
	char	*prompt;
	const char	*pwd;

	pwd = get_short_pwd(gc);
	if (!pwd || !get_username(gc))
		pwd = "~";
	prompt = ft_malloc(gc, 1024);
	if (!prompt)
		return (NULL);
	prompt[0] = '\0';
	add_color(prompt, BOLD_CYAN, 1024);
	ft_strlcat(prompt, get_username(gc), 1024);
	add_color(prompt, RESET, 1024);
	ft_strlcat(prompt, "@", 1024);
	add_color(prompt, BOLD_BLUE, 1024);
	ft_strlcat(prompt, pwd, 1024);
	add_color(prompt, RESET, 1024);
	ft_strlcat(prompt, "\n", 1024);
	if (exit_code == 0)
		add_color(prompt, BOLD_GREEN, 1024);
	else if (exit_code == 130)
		add_color(prompt, "\033[1;33m", 1024);
	else
		add_color(prompt, BOLD_RED, 1024);
	ft_strlcat(prompt, "\001➜\002 \x7f", 1024);
	add_color(prompt, RESET, 1024);
	return (prompt);
}
