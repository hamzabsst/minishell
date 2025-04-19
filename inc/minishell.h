/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/19 15:29:59 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include "../mylib/myLib.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct	s_command
{
	char	**argv;
	char	*infile;
	char	*outfile;
	int		append;
	int		heredoc;
	char	*delimiter;
	struct s_command *next;
}	t_command;

typedef struct s_env
{
	char	**envp;
} t_env;

//pasring
//
//
//excution
int	is_builtin(char *cmd);
int	exec_builtin(t_command *cmd, char **envp);
int	builtin_echo(char **argv);


#endif
