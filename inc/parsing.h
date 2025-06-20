/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:34:55 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/20 10:02:36 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../mylib/myLib.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "memory.h"

typedef struct s_cmd
{
	char			**av;
	char			**env;
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

void 	init_struct(t_cmd *cmd);
char	**smart_split(t_cmd *cmd, char *str);
void	add_cmd_back(t_cmd **lst, t_cmd *new);
void	add_token_back(t_token **head, t_token *new);
t_token	*allocate_token(t_cmd *cmd, char *content, char *type);
t_token	*tokenize(t_cmd *cmd, char **tokens);
t_cmd	*start_of_parsing(t_cmd *cmd, t_token *tokens);
char 	*insidequotes(t_cmd *cmd, char *line, int *i);
int		handle_quotes_error(char *line);
void	print_tokens(t_token *tokens);

//utils
char	*our_strjoin(t_mem *collector, char const *s1, char const *s2);
char	*our_strdup(t_mem *collector ,const char *s);
char	*our_strndup(t_mem *collector, char *str, size_t len, char skip_single_q, char skip_double_q);

#endif
