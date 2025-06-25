/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 10:50:24 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/25 09:16:26 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include "../mylib/myLib.h"

typedef struct s_mem_node
{
	void				*ptr;
	struct s_mem_node	*next;
}	t_mem_node;

typedef struct s_mem
{
	t_mem_node	*head;
}	t_mem;

void	init_mem(t_mem *manager);
void	*ft_malloc(t_mem *manager, size_t size);
void	ft_free_all(t_mem *manager);
void	ft_free_ptr(t_mem *manager, void *ptr);
int		ft_add_ptr(t_mem *manager, void *ptr);


#endif
