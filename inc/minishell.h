/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/19 21:18:05 by hbousset         ###   ########.fr       */
=======
/*   Updated: 2025/04/19 21:51:09 by abchaman         ###   ########.fr       */
>>>>>>> 6412ebdca851137cd14d35d0977ed70afbb71173
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include "../mylib/myLib.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct	s_cmd
{
	char	**argv;
	char	*infile;
	char	*outfile;
	int		append;
	int		heredoc;
	char	*delimiter;
	struct s_cmd *next;
}	t_cmd;

typedef struct s_env
{
	char	**envp;
} t_env;

//pasring
//
//
//excution
<<<<<<< HEAD
int	builtin(char *cmd);
int	exec_builtin(t_command *cmd, char **envp);
=======
int	is_builtin(char *cmd);
int	exec_builtin(t_cmd *cmd, char **envp);
int	builtin_echo(char **argv);
>>>>>>> 6412ebdca851137cd14d35d0977ed70afbb71173


#endif
