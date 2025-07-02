/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:34:55 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/02 02:50:18 by hbousset         ###   ########.fr       */
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

t_cmd	*parse_input(char *line, t_env *g_env, int *exit_code, t_mem *gc);
t_token	*tokenize(t_mem *gc, char **tokens);
char	**mysplit(char *str, t_mem *gc);
int		check_syntax_error(t_token *tokens, t_mem *gc);
void	check_quotes(t_token **tokens, t_mem *gc);
void	get_exit(t_token **tokens, int exit_code, t_mem *gc);
void	add_token(t_token **start, t_token **end, char *content,
			const char *type, t_mem *gc);
void	replace_token(t_token **tokens, t_token *curr, t_token *new_list, t_token *end);

//utils
char	*our_strjoin(t_mem *gc, char const *s1, char const *s2);
char	*our_strdup(t_mem *gc, const char *s);
char	*our_substr(char const *s, unsigned int start, size_t len, t_mem *gc);

#endif
