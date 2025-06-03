/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/03 22:58:44 by hbousset         ###   ########.fr       */
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

static int		g_sig = 0;
typedef struct s_cmd
{
	char			**av;
	char			**infiles;
	char			**outfiles;
	int				*append_flags;
	char			*heredoc;
	char			*delimiter;
	t_mem			*collector;
	int				*quote_flags;
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
char	**smart_split(t_cmd *cmd, char *str);
void	add_cmd_back(t_cmd **lst, t_cmd *new);
void	add_token_back(t_token **head, t_token *new);
t_token	*allocate_token(t_cmd *cmd, char *content, char *type);
t_token	*tokenize(t_cmd *cmd, char **tokens);
t_cmd	*start_of_parsing(t_cmd *cmd, t_token *tokens);
char 	*insidequotes(t_cmd *cmd, char *line, int *i);
int		handle_quotes_error(char *line);
void	print_tokens(t_token *tokens);

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
int	builtin_unset(char **av, char ***env, t_mem *collector);
int		builtin_exit(char **av, char ***env, t_mem *collector);

//exceve
int		ft_exec(t_cmd *cmd, char **env, t_mem *collector);
void	exec_cmd(t_cmd *cmd, char **env, t_mem *collector);
void	cleanup_child(t_mem *collector);
int		redirection(t_cmd *cmd, t_mem *collector);

//utils
char	*create_prompt(t_mem *collector);
int		ft_perror(char *msg);
char	*find_key(const char *arg);
char	*ft_strjoin_mem(t_mem *collector, char const *s1, char const *s2);
char	*ft_strdup_mem2(t_cmd *cmd, const char *s);
char	*ft_strdup_mem2(t_cmd *cmd, const char *s);
char	*ft_strndup_mem(t_cmd *cmd, char *str, size_t len, char skip_single_q, char skip_double_q);

//signal
void	handle_sigint(int sig);

#endif
