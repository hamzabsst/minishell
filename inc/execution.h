/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:35:14 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/25 17:14:39 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../mylib/myLib.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>

//env
t_env	*dup_env(char **env, t_mem *gc);
t_env	*allocate_var(const char *var, const char *content, t_mem *gc);
void	add_var_back(t_env **head, t_env *new);
char 	**env_to_array(t_cmd *cmd);
int		update_env(t_cmd *cmd, const char *key, const char *value);
char	*ft_getenv(t_env *env, const char *key);

//built-in cmds
int		builtin(char *av);
int		exec_builtin(t_cmd *cmd);
int		builtin_echo(t_cmd *cmd);
int		builtin_cd(t_cmd *cmd);
int		builtin_export(t_cmd *cmd);
int		builtin_unset(t_cmd *cmd);
int		builtin_exit(t_cmd *cmd);

//exceve
int		process_command(t_cmd *cmd);
int		ft_exec(t_cmd *cmd);
void	exec_cmd(t_cmd *cmd);
int		redirection(t_cmd *cmd);
void	cleanup_child(t_mem *gc);

//heredoc
char	*heredoc(t_cmd *cmd, t_mem *gc, int *index);
void	generate_filename(char *dest, size_t size, int index);
int		tmp_to_heredoc(char *filename);
void	handle_heredoc_sigint(int sig);
void	clean_heredoc(int fd, const char *path, int in, void (*handler)(int));

//utils
void	restore_io(int in_copy, int out_copy);
char	*create_prompt(t_mem *gc, int exit_code);
int		ft_perror(const char *msg);


#endif
