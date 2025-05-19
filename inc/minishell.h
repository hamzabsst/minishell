/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/19 10:39:21 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../mylib/myLib.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <stdbool.h>

typedef struct s_cmd
{
	char			**av;
	char			**infiles;
	char			**outfiles;
	int				*append_flags;
	char			*heredoc;
	char			*delimiter;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_token
{
	char			*content;		//"echo" ">" "hello"
	char			*type;			//"WORD" "REDIR_IN" "REDIR_OUT"
	struct s_token	*next;
}	t_token;

//parsing
void	init_struct(t_cmd *cmd);
char	**smart_split(char *str);
void	ft_lstadd_backk(t_cmd **lst, t_cmd *new);
void	print_tokens(t_token *tokens);
void	add_token_back(t_token **head, t_token *new);
t_token	*allocate_token(char *content, char *type);
t_token	*tokenize(char **tokens);
t_cmd	*start_of_parsing(t_token *tokens);
void	free_token_list(t_token *token);

//env
int		update_env(char ***env_ptr, char *key, char *value);
int		update_env_append(char ***env, char *key, char *value);
char	**dup_env(char **env);
char	*ft_getenv(char **env, const char *key);

//built-in cmds
int		exec_builtin(t_cmd *cmd, char ***env);
int		builtin(char *cmd);
int		builtin_echo(char **av);
int		builtin_cd(char **av, char ***env);
int		builtin_export(char **av, char ***env);
int		builtin_unset(char **av, char ***env);
int		builtin_exit(char **av, char ***env);

//exceve
int		ft_exec(t_cmd *cmd, char **env);
void	exec_cmd(t_cmd *cmd, char **env);
void	redirection(t_cmd *cmd);

//utils
int		ft_perror(char *msg);
char	*find_key(const char *arg);

//signal
void	handle_sigint(int sig);

#endif
