/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:34:55 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/04 15:47:21 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

//init_parse
t_cmd	*parse_input(char *line, t_env *g_env, int *exit_code, t_mem *gc);
void	init_cmds(t_cmd *cmd, t_parse *data, int exit_code);

//split
char	**mysplit(t_parse *data);
int		count_token(char *s);

//tokens
t_token	*tokenize(t_parse *data);
t_cmd	*process_tokens(t_cmd *head, t_parse *data, int *exit_code);
int		parse_heredoc(t_token **tokens, t_cmd *current, int *count, int *code);
int		parse_redir(t_token **tokens, t_cmd *current);

//check sytnax
int		check_syntax_error(t_parse *data);
void	handle_quotes(t_split *s);
void	check_quotes(t_token **tokens, t_mem *gc);
int		check_quotes_syntax(t_token *tokens);

//exit_code
void	get_exit(t_token **tokens, int exit_code, t_mem *gc);
void	add_token(t_token **start, char *content, const char *type, t_mem *gc);
void	repl_token(t_token **token, t_token *curr, t_token *new, t_token *end);

//utils
char	*our_strjoin(t_mem *gc, char const *s1, char const *s2);
char	*our_strdup(t_mem *gc, const char *s);
char	*our_substr(char const *s, unsigned int start, size_t len, t_mem *gc);

#endif
