/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset < hbousset@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:21:35 by hbousset          #+#    #+#             */
/*   Updated: 2025/01/16 18:00:33 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myLib.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*results;

	results = malloc(count * size);
	if (!results)
		return (NULL);
	ft_bzero(results, count * size);
	return (results);
}
