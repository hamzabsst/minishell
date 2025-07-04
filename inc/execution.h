/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:35:14 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/04 15:49:51 by hbousset         ###   ########.fr       */
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
void	restore_io(int in_copy, int out_copy);

//childs
pid_t	init_childs(t_cmd *cmd, int fd_in, int *pipe_fd);
char	*get_cmd_path(t_cmd *cmd, char **env);

//heredoc
char	*heredoc(t_cmd *cmd, int *index, int *exit_code);
void	generate_filename(char *dest, int index);
char	*remove_quotes(t_cmd *cmd);
int		tmp_to_heredoc(t_cmd *cmd);

//garabage collector
void	*ft_malloc(t_mem *manager, size_t size);
int		ft_add_ptr(t_mem *manager, void *ptr);
void	ft_free_all(t_mem *manager);

//utils
char	*create_prompt(t_mem *gc, int exit_code);

//error messages
int		ft_error(const char *prefix, const char *token,
			const char *suffix, t_mem *gc);
int		our_error(const char *msg);

#endif
