/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:34:55 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/03 01:34:38 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdbool.h>

typedef struct s_env
{
	char			*var;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd
{
	t_env			*env;
	t_mem			*gc;
	char			**av;
	char			**infiles;
	char			**outfiles;
	bool			*append_flags;
	char			*heredoc;
	char			*delimiter;
	bool			heredoc_processed;
	bool			forked;
	int				exit_code;
	int				in_copy;
	int				out_copy;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_token
{
	char			*content;
	char			*type;
	struct s_token	*next;
}	t_token;

typedef struct s_parse
{
	char	*line;
	char	**splited;
	t_token	*tokens;
	t_env	*env;
	t_mem	*gc;
}	t_parse;

typedef struct s_split
{
	char	**word;
	char	*line;
	t_mem	*gc;
	int		to_join;
	int		last;
	int		i;
	int		j;
	int		len;
}	t_split;

void	init_cmds(t_cmd *cmd, t_parse *data, int exit_code);
t_cmd	*process_tokens(t_cmd *head, t_parse *data, int *exit_code);
int		check_quotes_syntax(t_token *tokens);
int		parse_heredoc(t_token **tokens, t_cmd *current, int *count, int *code);
int		parse_redir(t_token **tokens, t_cmd *current);
t_cmd	*parse_input(char *line, t_env *g_env, int *exit_code, t_mem *gc);
int		count_token(char *s);
char	**mysplit(t_parse *data);
t_token	*tokenize(t_parse *data);
void	handle_quotes(t_split *s);
int		check_syntax_error(t_parse *data);
void	check_quotes(t_token **tokens, t_mem *gc);
void	get_exit(t_token **tokens, int exit_code, t_mem *gc);
void	add_token(t_token **start, char *content, const char *type, t_mem *gc);
void	repl_token(t_token **token, t_token *curr, t_token *new, t_token *end);

//utils
char	*our_strjoin(t_mem *gc, char const *s1, char const *s2);
char	*our_strdup(t_mem *gc, const char *s);
char	*our_substr(char const *s, unsigned int start, size_t len, t_mem *gc);

#endif
