/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/20 10:18:11 by hbousset         ###   ########.fr       */
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

int	builtin(char *cmd);
int	exec_builtin(t_cmd *cmd, char **envp);
int	builtin_cd(char **argv, char ***env_ptr);
#endif
