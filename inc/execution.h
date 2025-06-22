/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:35:14 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/22 11:29:31 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../mylib/myLib.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>

//env
t_expand	*dup_env(char **env, t_mem *gc);
t_expand	*allocate_var(char *var, char *content, t_mem *gc);
void		add_var_back(t_expand **head, t_expand *new);
char 		**env_to_array(t_cmd *cmd);
int 		update_env(t_cmd *cmd, char *key, char *value);

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
int		redirection(t_cmd *cmd);
void	cleanup_child(t_mem *gc);

//heredoc
char	*heredoc(t_cmd *cmd, t_mem *gc, int *index);

//utils
char	*our_substr(char const *s, unsigned int start, size_t len, t_mem *gc);
int		ft_perror(char *msg);


#endif
