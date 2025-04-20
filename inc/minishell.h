/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 09:48:57 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/20 13:37:02 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include "../mylib/myLib.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct	s_cmd
{
	char	**argv;
	char	*infile;
	char	*outfile;
	int		append;
	int		heredoc;
	char	*delimiter;
	struct s_cmd *next;
}	t_cmd;

typedef struct s_env
{
	char	**envp;
} t_env;

//pasring
t_cmd	*ft_lstlastt(t_cmd *lst);
void	ft_lstadd_backk(t_cmd **lst, t_cmd *new);
void init_struct(t_cmd *cmd);
char **split_pipe(char *line);
void read_token(char **commands, t_cmd *cmd_list);
//
//excution
int	builtin_cd(char **argv, char ***env_ptr);
int	builtin(char *cmd);
int	exec_builtin(t_cmd *cmd, char **envp);
#endif
