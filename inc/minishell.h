/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/23 11:11:31 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../mylib/myLib.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <stdbool.h>

#define RED     "\001\033[1;31m\002"
#define GREEN   "\001\033[1;32m\002"
#define YELLOW  "\001\033[1;33m\002"
#define BLUE    "\001\033[1;34m\002"
#define MAGENTA "\001\033[1;35m\002"
#define CYAN    "\001\033[1;36m\002"
#define RESET   "\001\033[0m\002"

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
int		builtin_echo(char **av);
int		builtin_cd(char **av, char ***env);
int		builtin_export(char **av, char **env);
int		builtin_unset(char **av, char ***env);
int		builtin_exit(char **argv, char ***env);
int		exec_pipeline(t_cmd *cmd, char **env);
void	exec_cmd(t_cmd *cmd, char **env);
void	handle_redirection(t_cmd *cmd);


#endif

