/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:35:14 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/21 12:03:17 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../mylib/myLib.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>

//env
char 		**env_to_array(t_cmd *cmd);
int 		update_env(t_cmd *cmd, char *key, char *value);
int			update_env_append(t_cmd *cmd, char *key, char *value);
t_expand	*dup_env(char **env, t_mem *gc);
char		*find_key(const char *arg, t_mem *gc);
void		add_expand_back(t_expand **head, t_expand *new);
t_expand	*allocate_expand(char *var, char *content, t_mem *gc);
int			parse_env(const char *env, char **var, char **value, t_mem *gc);
char		*extract_var_name(const char *str, t_mem *gc);
char		*get_from_env(t_expand *env, char *str, t_mem *gc);
char 		*ft_getenv(t_expand *env, const char *key);

//built-in cmds
int		exec_builtin(t_cmd *cmd);
int		builtin(char *cmd);
int		builtin_echo(t_cmd *cmd);
int		builtin_cd(t_cmd *cmd);
int		builtin_export(t_cmd *cmd);
int		builtin_unset(t_cmd *cmd);
int		builtin_exit(t_cmd *cmd);

//exceve
int		ft_exec(t_cmd *cmd);
void	exec_cmd(t_cmd *cmd);
void	cleanup_child(t_mem *gc);
int		redirection(t_cmd *cmd);

//utils
char	*our_substr(char const *s, unsigned int start, size_t len, t_mem *gc);
int		ft_perror(char *msg);

//heredoc
char	*heredoc(t_cmd *cmd, t_mem *gc, int *index);

#endif
