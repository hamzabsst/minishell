/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 10:51:50 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/02 01:54:23 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_malloc(t_mem *gc, size_t size)
{
	void		*ptr;
	t_mem_node	*new_node;

	if (!gc || size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, size);
	new_node = malloc(sizeof(t_mem_node));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->ptr = ptr;
	new_node->next = gc->head;
	gc->head = new_node;
	return (ptr);
}

int	ft_add_ptr(t_mem *gc, void *ptr)
{
	t_mem_node	*new_node;

	if (!gc || !ptr)
		return (0);
	new_node = malloc(sizeof(t_mem_node));
	if (!new_node)
		return (0);
	new_node->ptr = ptr;
	new_node->next = gc->head;
	gc->head = new_node;
	return (1);
}

void	ft_free_ptr(t_mem *gc, void *ptr)
{
	t_mem_node	*current;
	t_mem_node	*prev;

	if (!gc || !ptr)
		return ;
	current = gc->head;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				gc->head = current->next;
			free(current->ptr);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_free_all(t_mem *gc)
{
	t_mem_node	*current;
	t_mem_node	*next;

	if (!gc)
		return ;
	current = gc->head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	gc->head = NULL;
}
