/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:49:53 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/14 00:29:37 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myLib.h"

int	ft_putstr_fd(const char *s, int fd)
{
	if (fd < 0 || !s)
		return (1);
	if (write(fd, s, ft_strlen(s)) == -1)
	{
		write(2, "write error: Bad file descriptor\n", 33);
		return (1);
	}
	return (0);
}
