/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/22 11:02:23 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../mylib/myLib.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <stdbool.h>

typedef struct s_token
{
    char *content;         // "echo" ">" "hello"
    char *type;            // "WORD" "REDIR_IN" "REDIR_OUT" ...
    struct s_token *next;
}   t_token;

typedef struct s_cmd
{
	char			**av; //{"cat", "file.txt", NULL}
	char			*infile; // <
	char			*outfile; // >
	int				append; //1 if >>
	char			*heredoc; // >>
	struct s_cmd	*next;
}	t_cmd;

//parsing
void	init_struct(t_cmd *cmd);
char	**smart_split(char *str);
void	ft_lstadd_backk(t_cmd **lst, t_cmd *new);
void 	print_tokens(t_token *tokens);
void add_token_back(t_token **head, t_token *new);
t_token *allocate_token(char *content, char *type);
t_token *tokenize(char **tokens);

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

