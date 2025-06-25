/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/25 12:06:33 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define RESET		"\033[0m"
#define RED			"\033[0;31m"
#define BOLD_BLUE	"\033[1;34m"
#define BOLD_CYAN	"\033[1;36m"
#define BOLD_GREEN	"\033[1;32m"
#define BOLD_RED	"\033[1;31m"

# include "parsing.h"
# include "execution.h"
# include "memory.h"

static int g_var;

#endif
