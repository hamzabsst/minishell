/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:34:27 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/05 10:46:19 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_short_pwd(t_mem *collector)
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

char	*create_prompt(t_mem *collector)
{
	char	*prompt;
	char	*pwd;

	pwd = get_short_pwd(collector);
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
