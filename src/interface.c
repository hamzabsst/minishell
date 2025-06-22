/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:34:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/22 11:31:01 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *get_short_pwd(t_mem *gc)
{
	char	*short_pwd;
	char	*cwd;
	char	*home;
	char	*last_slash;
	size_t	home_len;

	cwd = malloc(1024);
	if (cwd != NULL)
		getcwd(cwd, 1024);
	home = getenv("HOME");
	short_pwd = ft_malloc(gc, 256);
	if (!short_pwd)
		return (NULL);
	if (!cwd)
	{
		ft_strlcpy(short_pwd, "->  ~", 256);
		return (short_pwd);
	}
	if (home && strstr(cwd, home) == cwd)
	{
		home_len = strlen(home);
		ft_strlcpy(short_pwd, "->  ~", 256);
		ft_strlcat(short_pwd, cwd + home_len, 256);
	}
	else
	{
		ft_strlcpy(short_pwd, "->  ", 256);
		ft_strlcat(short_pwd, cwd, 256);
	}
	free(cwd);
	if (strlen(short_pwd) > 28)
	{
		last_slash = strrchr(short_pwd, '/');
		if (last_slash && last_slash != short_pwd)
		{
			ft_strlcpy(short_pwd, "->  ...", 256);
			ft_strlcat(short_pwd, last_slash, 256);
		}
	}
	return (short_pwd);
}

static void add_color(char *prompt, const char *color, size_t size)
{
	char temp[64];

	ft_strlcpy(temp, "\001", sizeof(temp));
	ft_strlcat(temp, color, sizeof(temp));
	ft_strlcat(temp, "\002", sizeof(temp));
	ft_strlcat(prompt, temp, size);
}

char	*create_prompt(t_mem *gc)
{
	char	*prompt;
	char	*pwd;

	pwd = get_short_pwd(gc);
	if (!pwd)
		return (NULL);
	prompt = ft_malloc(gc, 512);
	if (!prompt)
		return (NULL);
	prompt[0] = '\0';
	add_color(prompt, "\033[1;34m", 512);
	ft_strlcat(prompt, pwd, 512);
	ft_strlcat(prompt, " ", 512);
	add_color(prompt, "\033[0m", 512);
	return (prompt);
}
