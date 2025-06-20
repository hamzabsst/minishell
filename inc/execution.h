/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:35:14 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/20 11:23:46 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../mylib/myLib.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct s_mem t_mem;
typedef struct s_cmd t_cmd;

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
int		builtin_unset(char **av, char ***env, t_mem *collector);
int		builtin_exit(char **av, char ***env, t_mem *collector);

//exceve
int		ft_exec(t_cmd *cmd, char **env, t_mem *collector);
void	exec_cmd(t_cmd *cmd, char **env, t_mem *collector);
void	cleanup_child(t_mem *collector);
int		redirection(t_cmd *cmd);

//utils
int		ft_perror(char *msg);
char	*find_key(const char *arg);

//heredoc
char	*heredoc(t_cmd *cmd, t_mem *collector, int *index);

#endif
