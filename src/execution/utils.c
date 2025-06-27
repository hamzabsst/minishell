/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:39:41 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/25 14:58:05 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*our_substr(char const *s, unsigned int start, size_t len, t_mem *gc)
{
	char	*substr;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (our_strdup(gc, ""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	substr = ft_malloc(gc, len * sizeof(char) + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (len > i && s[start + i])
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*our_strndup(t_mem *gc, const char *str, size_t len, char skip_single_q, char skip_double_q)
{
	size_t	i;
	char	*results;
	int		k;

	k = 0;
	results = ft_malloc(gc, len + 1);
	if (!results)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if(str[i] != skip_single_q && str[i] != skip_double_q)
			results[k++] = str[i];
		i++;
	}
	results[k] = '\0';
	return (results);
}

char	*our_strdup(t_mem *gc ,const char *s)
{
	size_t	i;
	size_t	size;
	char	*results;

	size = ft_strlen(s);
	results = ft_malloc(gc, (size + 1) * sizeof(char));
	if (!results)
		return (NULL);
	i = 0;
	while (s[i])
	{
		results[i] = s[i];
		i++;
	}
	results[i] = '\0';
	return (results);
}

char	*our_strjoin(t_mem *gc, char const *s1, char const *s2)
{
	char	*res;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	res = ft_malloc(gc , ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s1))
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < ft_strlen(s2))
	{
		res[i++] = s2[j++];
	}
	res[i] = '\0';
	return (res);
}
