/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:36 by hbousset          #+#    #+#             */
<<<<<<< HEAD:src/excution/main.c
/*   Updated: 2025/04/19 15:35:22 by hbousset         ###   ########.fr       */
=======
<<<<<<< HEAD
/*   Updated: 2025/04/19 15:16:35 by abchaman         ###   ########.fr       */
=======
/*   Updated: 2025/04/19 14:21:35 by hbousset         ###   ########.fr       */
>>>>>>> 79d8ee84f90883a8b384d8957bc4b2eb8dbabe87
>>>>>>> b0052bdb18eccecb1e57cb7322516dce4ec02562:src/hamza/main.c
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int main(int ac, char **av, char **env)
{

	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments.\n", 36);
		exit(1);
	}
	t_command cmd = {
		.argv = (char *[]){"echo", "-n", "hello", "world", NULL},
		.infile = NULL,
		.outfile = NULL,
		.append = 0,
		.heredoc = 0,
		.delimiter = NULL,
		.next = NULL
	};

	if (is_builtin(cmd.argv[0]) && !cmd.next)
		return (exec_builtin(&cmd, env));
	if (*av[5] == 5)
		return (1);
	return (0);
}

