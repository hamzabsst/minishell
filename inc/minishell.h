/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/22 10:55:09 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../mylib/myLib.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <stdbool.h>

typedef struct s_cmd
{
	char			**av;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	char			*delimiter;
	struct s_cmd	*next;
}	t_cmd;

//parsing
void	init_struct(t_cmd *cmd);
char	**split_pipe(char *line);
void	read_token(char **commands, t_cmd *cmd_list);
char	**smart_split(char *str);

//excution
char	**dup_env(char **env);
int		builtin(char *cmd);
int		exec_builtin(t_cmd *cmd, char ***env);
int		builtin_cd(char **av, char ***env);
int		builtin_export(char **av, char **env);
int		builtin_unset(char **av, char ***env);
int		builtin_exit(char **argv, char ***env);
int		exec_pipeline(t_cmd *cmd, char **env);
void	exec_cmd(t_cmd *cmd, char **env);

#endif

