/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/05/29 11:43:55 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


#define RESET	"\033[0m"
#define BOLD_BLUE    "\033[1;34m"
#define BRIGHT_BLUE     "\033[94m"
#define BRIGHT_CYAN     "\033[96m"
#define BRIGHT_MAGENTA  "\033[95m"

# include "../mylib/myLib.h"
# include "ft_mem.h"
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
	t_mem			*collector;
}	t_cmd;

typedef struct s_token
{
	char			*content;		//"echo" ">" "hello"
	char			*type;			//"WORD" "REDIR_IN" "REDIR_OUT"
	struct s_token	*next;
}	t_token;

//parsing
char 	*ft_strndup(t_cmd *cmd, char *str, size_t len, char skip);
void	init_struct(t_cmd *cmd);
char	**smart_split(t_cmd *cmd, char *str);
void	ft_lstadd_backk(t_cmd **lst, t_cmd *new);
void	print_tokens(t_token *tokens);
void	add_token_back(t_token **head, t_token *new);
t_token	*allocate_token(t_cmd *cmd, char *content, char *type);
t_token	*tokenize(t_cmd *cmd, char **tokens);
t_cmd	*start_of_parsing(t_cmd *cmd, t_token *tokens);
char 	*insidequotes(t_cmd *cmd, char *line, int *i);

//env
int		update_env(char ***env_ptr, char *key, char *value, t_mem *collector);
int		update_env_append(char ***env, char *key, char *value, t_mem *collector);
char	**dup_env(char **env, t_mem *collector);
char	*ft_getenv(char **env, const char *key);

//built-in cmds
int		exec_builtin(t_cmd *cmd, char ***env, t_mem *collector);
int		builtin(char *cmd);
int		builtin_echo(char **av);
int		builtin_cd(char **argv, char ***env, t_mem *collector);
int		builtin_export(char **av, char ***env, t_mem *collector);
int		builtin_unset(char **av, char ***env);
int		builtin_exit(char **av, char ***env, t_mem *collector);

//exceve
int		ft_exec(t_cmd *cmd, char **env, t_mem *collector);
void	exec_cmd(t_cmd *cmd, char **env, t_mem *collector);
void	redirection(t_cmd *cmd, t_mem *collector);
void	cleanup_child(t_mem *collector);

//utils
int		ft_perror(char *msg);
char	*find_key(const char *arg);

//signal
void	handle_sigint(int sig);

#endif
