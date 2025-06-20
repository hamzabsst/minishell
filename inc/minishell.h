/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/05 22:03:30 by hbousset         ###   ########.fr       */
=======
/*   Updated: 2025/06/20 10:16:15 by hbousset         ###   ########.fr       */
>>>>>>> hamza
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

<<<<<<< HEAD

#define RESET	"\001\033[0m\002"
#define BOLD_BLUE    "\001\033[1;34m\002"
#define BRIGHT_BLUE     "\001\033[94m\002"
#define BRIGHT_CYAN     "\001\033[96m\002"
#define BRIGHT_MAGENTA  "\001\033[95m\002"
=======
#define RESET			"\033[0m"
#define BOLD_BLUE		"\033[1;34m"
#define BRIGHT_BLUE		"\033[94m"
#define BRIGHT_CYAN		"\033[96m"
#define BRIGHT_MAGENTA	"\033[95m"
>>>>>>> hamza

# include "parsing.h"
# include "execution.h"
# include "memory.h"

static int		g_sig;

//interface
char	*create_prompt(t_mem *collector);

//signal
void	handle_sigint(int sig);

#endif
