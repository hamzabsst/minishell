/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/20 11:46:32 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define RESET			"\033[0m"
#define BOLD_BLUE		"\033[1;34m"
#define BRIGHT_BLUE		"\033[94m"
#define BRIGHT_CYAN		"\033[96m"
#define BRIGHT_MAGENTA	"\033[95m"

# include "parsing.h"
# include "execution.h"
# include "memory.h"

static int		g_sig;

//interface
char	*create_prompt(t_mem *collector);

//signal
void	handle_sigint(int sig);

#endif
