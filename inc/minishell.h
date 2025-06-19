/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/18 19:48:41 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


#define RESET	"\001\033[0m\002"
#define BOLD_BLUE    "\001\033[1;34m\002"
#define BRIGHT_BLUE     "\001\033[94m\002"
#define BRIGHT_CYAN     "\001\033[96m\002"
#define BRIGHT_MAGENTA  "\001\033[95m\002"

# include <stdio.h>
# include "../mylib/myLib.h"
# include "ft_mem.h"
# include <signal.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

static int		g_sig = 0;

typedef struct s_expand
{
	char			*var;		//"echo" ">" "hello"
	char			*content_of_var;			//"WORD" "REDIR_IN" "REDIR_OUT"
	struct s_expand	*next;
}	t_expand;

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
	t_expand		*env;
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
// char	**smart_split(t_cmd *cmd, char *str);
char	*get_from_dquotes(char *str, int *start, int *last);
char	*get_from_squotes(char *str, int *start, int *last);
char	*simple_word(char *str, int *start, int *last);
char	*get_redirection(char *str, int *start, int *last);
char	*get_pipe(char *str, int *start, int *last);
char **mysplit(t_cmd *cmd, char *str);
void	add_cmd_back(t_cmd **lst, t_cmd *new);
void	add_token_back(t_token **head, t_token *new);
t_token	*allocate_token(t_cmd *cmd, char *content, char *type);
t_token	*tokenize(t_cmd *cmd, char **tokens);
t_cmd	*start_of_parsing(t_cmd *cmd, t_token *tokens);
// char 	*insidequotes(t_cmd *cmd, char *line, int *i);
int		handle_quotes_error(char *line);
void	print_tokens(t_token *tokens);
// void	add_expand_back(t_expand **head, t_expand *new);
// t_expand	*allocate_expand(t_cmd *cmd, char *var, char *content_of_var);
// t_expand	*copy_from_env(t_cmd *cmd, char **env);
// char	*get_from_env(t_cmd *cmd ,t_expand *env, char *str);
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
char	*our_strjoin(t_mem *collector, char const *s1, char const *s2);
char	*our_strdup(t_mem *collector ,const char *s);
char	*our_strndup(t_mem *collector, char *str, size_t len);

//signal
void	handle_sigint(int sig);

#endif
