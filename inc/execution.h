/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:35:14 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/02 14:54:29 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

//env
t_env	*dup_env(char **env, t_mem *gc);
t_env	*allocate_var(const char *var, const char *content, t_mem *gc);
void	add_var_back(t_env **head, t_env *new);
char	**env_to_array(t_cmd *cmd);
int		update_env(t_cmd *cmd, const char *key, const char *value);
char	*ft_getenv(t_env *env, const char *key);

//built-in cmds
int		builtin(char *av);
int		builtin_pwd(void);
int		builtin_env(t_cmd *cmd);
int		builtin_echo(t_cmd *cmd);
int		builtin_cd(t_cmd *cmd);
int		builtin_unset(t_cmd *cmd);
int		builtin_exit(t_cmd *cmd);
int		builtin_export(t_cmd *cmd);
void	process_av(t_cmd *cmd, char *arg);

//exceve
int		process_command(t_cmd *cmd);
int		exec_builtin(t_cmd *cmd);
int		init_exec(t_cmd *cmd);
int		redirection(t_cmd *cmd);

//childs
pid_t	init_childs(t_cmd *cmd, int fd_in, int *pipe_fd);
char	*get_cmd_path(t_cmd *cmd, char **env);

//heredoc
void	generate_filename(char *dest, int index);
char	*remove_quotes(t_cmd *cmd);
char	*heredoc(t_cmd *cmd, int *index, int *exit_code);
int		tmp_to_heredoc(t_cmd *cmd);

//utils
void	restore_io(int in_copy, int out_copy);
char	*create_prompt(t_mem *gc, int exit_code);
int		ft_error(const char *prefix, const char *token,
			const char *suffix, t_mem *gc);
int		our_perror(const char *msg);

#endif
