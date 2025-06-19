/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:57:49 by abchaman          #+#    #+#             */
/*   Updated: 2025/06/18 19:30:50 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_token
{
	char			*content;		//"echo" ">" "hello"
	char			*type;			//"WORD" "REDIR_IN" "REDIR_OUT"
	struct s_token	*next;
}	t_token;

char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	size;
	char	*results;

	size = strlen(s);
	results = (char *) malloc((size + 1) * sizeof(char));
	if (!results)
		return (NULL);
	i = 0;
	while (s[i])
	{
		results[i] = s[i];
		i++;
	}
	results[i] = '\0';
	return (results);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= strlen(s))
		return (ft_strdup(""));
	if (len > strlen(s) - start)
		len = strlen(s) - start;
	substr = (char *)malloc(len * sizeof(char) + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (len > i && s[start + i])
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
static int	count_token(char *str)
{
	int j = 0;
	int count = 0;
	int position = 0;
	int i = 0;
	char quote_type = '\0';

	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break;
		count++;
		if (str[i] == '\"' || str[i] == '\'')
		{
			quote_type = str[i++];
			j = i;
			while (str[j] && str[j] != ' ')
			{
				if (str[j] == quote_type)
					position = j;
				j++;
			}
			while (str[i] && i != position)
				i++;
			if (str[i] == quote_type)
				i++;
		}
		else if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
				i += 2;
			else
				i++;
		}
		else
			while (str[i] && str[i] != ' ' && str[i] != '>' && str[i] != '<' &&
				str[i] != '|' && str[i] != '\'' && str[i] != '\"')
				i++;
	}
	return (count);
}
char *get_from_dquotes(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int	pos;
	int len;

	pos = 0;
	i = *start + 1;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '"')
		{
			pos = i;
			*last = pos - *start + 1;
		}
		i++;
	}
	len = pos - *start + 1;
	word = ft_substr(str, *start, len);
	return(word);
}
char *get_from_squotes(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int	pos;
	int len;

	pos = 0;
	i = *start + 1;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '\'')
		{
			pos = i;
			*last = pos - *start;
		}
		i++;
	}
	len = pos - *start;
	word = ft_substr(str, *start, len);
	return(word);
}

char	*simple_word(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int len;

	i = *start;
	while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'' && str[i] != '>'
		&& str[i] != '<'  && str[i] != '|')
		i++;
	*last = i - *start;
	len = i - *start;
	word = ft_substr(str, *start, len);
	return(word);
}
char	*get_redirection(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int len;

	i = *start;
	if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			len = 2;
		else
			len = 1;
	}
	else if(str[i] == '<')
	{
		if (str[i + 1] == '<')
			len = 2;
		else
			len = 1;
	}
	*last = len + 1;
	word = ft_substr(str, *start, len);
	return(word);
}

char	*get_pipe(char *str, int *start, int *last)
{
	int i = 0;
	char *word;
	int len;

	i = *start;
	if (str[i] == '|')
		len = 1;
	else
		return (NULL);
	*last = len + 1;
	word = ft_substr(str, *start, (len));
	return(word);
}
char **mysplit(char *str)
{
	int	last = 0;
	int i = 0;
	int	k = 0;
	char **word = malloc(sizeof(char *) * count_token(str) + 1);
	if (!word)
		return (NULL);
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] == '"')
			word[k++] = get_from_dquotes(str, &i, &last);
		else if (str[i] == '\'')
			word[k++] = get_from_squotes(str, &i, &last);
		else if (str[i] == '>' || str[i] == '<')
			word[k++] = get_redirection(str, &i, &last);
		else if(str[i] == '|')
			word[k++] = get_pipe(str, &i, &last);
		else if (str[i] && str[i] != '"' && str[i] != '\'' && str[i] != '>' && str[i] != '<'  && str[i] != '|')
			word[k++] = simple_word(str, &i, &last);
		i += last;
	}
	word[k] = NULL;
	return (word);
}
void	print_tokens(t_token *tokens)
{
	int i = 0;
	while (tokens)
	{
		printf("Token[%d]: %s Type: %s\n", i, tokens->content, tokens->type);
		tokens = tokens->next;
		i++;
	}
}
void	add_token_back(t_token **head, t_token *new)
{
	t_token *tmp;
	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	tmp->next = new;
}

t_token	*allocate_token(char *content, char *type)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = ft_strdup(content);
	new->type = ft_strdup(type);
	new->next = NULL;
	return (new);
}
t_token	*tokenize(char **tokens)
{
	t_token	*head;
	int		i;
	char	*type;
	t_token *new;

	i = 0;
	head = NULL;
	while (tokens[i])
	{
		type = "WORD";
		if(i > 0 && strcmp(tokens[i - 1], "<<") == 0)
			type = "DELIMITER";
		else if (strcmp(tokens[i], "|") == 0)
			type = "PIPE";
		else if (strcmp(tokens[i], ">") == 0)
			type = "REDIRECTION_OUT";
		else if (strcmp(tokens[i], "<") == 0)
			type = "REDIRECTION_IN";
		else if(strcmp(tokens[i], ">>") == 0)
			type = "APPEND";
		else if (strcmp(tokens[i], "<<") == 0)
			type = "HEREDOC";
		new = allocate_token(tokens[i], type);
		add_token_back(&head, new);
		i++;
	}
	print_tokens(head);
	return (head);
}
int main()
{
	t_token *head = NULL;
	int i = 0;
	char str[] = "echo \"sdf\" \"|\" << d";
	// int c = count_token(str);
	char **out;
	out = mysplit(str);
	tokenize(out);
}
