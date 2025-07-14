/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:38:23 by hbousset          #+#    #+#             */
/*   Updated: 2025/07/14 00:12:09 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_env
{
	char			*var;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_mem_node
{
	void				*ptr;
	struct s_mem_node	*next;
}	t_mem_node;

typedef struct s_mem
{
	t_mem_node	*head;
}	t_mem;

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
	void			(*sigint)(int);
	void			(*sigquit)(int);
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

#endif
