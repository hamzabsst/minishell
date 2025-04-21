/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/21 10:56:43 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../mylib/myLib.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	char			*delimiter;
	struct s_cmd	*next;
}	t_cmd;

char	**dup_env(char **env);
int		builtin(char *cmd);
int		exec_builtin(t_cmd *cmd, char ***env);
int		builtin_cd(char **argv, char ***env);
int		builtin_export(char **argv, char **env);
int		builtin_unset(char **argv, char ***env);
int		exec_pipeline(t_cmd *cmd, char **env);

#endif
