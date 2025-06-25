/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:34:55 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/25 15:40:01 by hbousset         ###   ########.fr       */
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

typedef struct s_env
{
	char			*var;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd
{
	char			**av;
	char			**infiles;
	char			**outfiles;
	int				*append_flags;
	char			*heredoc;
	char			*delimiter;
	t_env			*env;
	t_mem			*gc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_token
{
	char			*content;		//"echo" ">" "hello"
	char			*type;			//"WORD" "REDIR_IN" "REDIR_OUT"
	struct s_token	*next;
}	t_token;

t_cmd	*parse_input(char *line, t_env *g_env, int exit_code, int *input, t_mem *gc);
int		check_syntax_error(t_token *tokens);
char	**mysplit(char *str, t_mem *gc);
void	get_exit(t_token **tokens, int exit_code, t_mem *gc);
t_token	*tokenize(t_cmd *cmd, char **tokens);
t_cmd	*start_of_parsing(t_cmd *cmd, t_token *tokens);
void	add_token(t_token **start, t_token **end, char *content, const char *type, t_mem *gc);
void	replace_token(t_token **tokens, t_token *curr, t_token *new_list, t_token *end);
void	check_quotes(t_token **tokens, t_mem *gc);

//utils
char	*our_strjoin(t_mem *gc, char const *s1, char const *s2);
char	*our_strdup(t_mem *gc ,const char *s);
char	*our_strndup(t_mem *gc, const char *str, size_t len, char skip_single_q, char skip_double_q);
char	*our_substr(char const *s, unsigned int start, size_t len, t_mem *gc);

#endif
