/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 10:51:50 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/03 22:17:17 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mem.h"

void	init_mem(t_mem *collector)
{
	if (collector)
		collector->head = NULL;
}

void *ft_malloc(t_mem *collector, size_t size)
{
	void		*ptr;
	t_mem_node	*new_node;

	if (!collector || size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new_node = malloc(sizeof(t_mem_node));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->ptr = ptr;
	new_node->next = collector->head;
	collector->head = new_node;
	return (ptr);
}

int	ft_add_ptr(t_mem *collector, void *ptr)
{
	t_mem_node	*new_node;

	if (!collector || !ptr)
		return (0);
	new_node = malloc(sizeof(t_mem_node));
	if (!new_node)
		return (0);
	new_node->ptr = ptr;
	new_node->next = collector->head;
	collector->head = new_node;
	return (1);
}

void	ft_free_ptr(t_mem *collector, void *ptr)
{
	t_mem_node	*current;
	t_mem_node	*prev;

	if (!collector || !ptr)
		return;
	current = collector->head;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				collector->head = current->next;
			free(current->ptr);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_free_all(t_mem *collector)
{
	t_mem_node	*current;
	t_mem_node	*next;

	if (!collector)
		return;
	current = collector->head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	collector->head = NULL;
}
