/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 10:51:50 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/25 11:49:23 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mem.h"

void	init_mem(t_mem *manager)
{
	if (manager)
		manager->head = NULL;
}

void *ft_malloc(t_mem *manager, size_t size)
{
	void		*ptr;
	t_mem_node	*new_node;

	if (!manager || size == 0)
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
	new_node->next = manager->head;
	manager->head = new_node;
	return (ptr);
}

int	ft_add_ptr(t_mem *manager, void *ptr)
{
	t_mem_node	*new_node;
	if (!manager || !ptr)
		return (0);
	new_node = malloc(sizeof(t_mem_node));
	if (!new_node)
		return (0);
	new_node->ptr = ptr;
	new_node->next = manager->head;
	manager->head = new_node;
	return (1);
}

void	ft_free_ptr(t_mem *manager, void *ptr)
{
	t_mem_node	*current;
	t_mem_node	*prev;

	if (!manager || !ptr)
		return;
	current = manager->head;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				manager->head = current->next;
			free(current->ptr);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_free_all(t_mem *manager)
{
	t_mem_node *current;
	t_mem_node *next;

	if (!manager)
		return;
	current = manager->head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	manager->head = NULL;
}
