/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:02:11 by abchaman          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/19 21:35:03 by hbousset         ###   ########.fr       */
=======
<<<<<<< HEAD
/*   Updated: 2025/04/19 21:51:09 by abchaman         ###   ########.fr       */
=======
/*   Updated: 2025/04/19 21:06:14 by hbousset         ###   ########.fr       */
>>>>>>> 5784d933ed07120410296bbaa5c786ba2528cb8e
>>>>>>> 6412ebdca851137cd14d35d0977ed70afbb71173
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int main(int ac, char **av, char **env)
{

	if (ac != 1)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		exit(1);
	}
<<<<<<< HEAD
	t_cmd cmd = {
		.argv = (char *[]){"echo", "-n", "hello", "qwd", NULL},
=======
	t_command cmd = {
<<<<<<< HEAD
		.argv = (char *[]){"cd", NULL},
=======
		.argv = (char *[]){"echo", "hello", "qwd", "nta", NULL},
>>>>>>> 5784d933ed07120410296bbaa5c786ba2528cb8e
>>>>>>> 6412ebdca851137cd14d35d0977ed70afbb71173
		.infile = NULL,
		.outfile = NULL,
		.append = 0,
		.heredoc = 0,
		.delimiter = NULL,
		.next = NULL
	};

	if (builtin(cmd.argv[0]) && !cmd.next)
		return (exec_builtin(&cmd, env));
	if (*av[5] == 5)
		return (1);
	return (0);
}

