/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:36 by hbousset          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/19 15:16:35 by abchaman         ###   ########.fr       */
=======
/*   Updated: 2025/04/19 14:21:35 by hbousset         ###   ########.fr       */
>>>>>>> 79d8ee84f90883a8b384d8957bc4b2eb8dbabe87
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	main(void)
{
	t_command	cmd1 = {
		.argv = (char *[]){"ls", "-l", NULL},
		.infile = NULL,
		.outfile = "out.txt",
		.append = 0,
		.heredoc = 0,
		.delimiter = NULL,
		.next = NULL
	};
}
