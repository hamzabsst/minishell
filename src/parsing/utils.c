/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:33:24 by abchaman          #+#    #+#             */
/*   Updated: 2025/04/22 11:00:46 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *ft_strndup(char *str, size_t len)
{
	size_t	i;
	char	*results;

	results = (char *) malloc((len + 1) * sizeof(char));
	if (!results)
		return (NULL);
	i = 0;
	while (i < len)
	{
		results[i] = str[i];
		i++;
	}
	results[i] = '\0';
	return (results);
}
static int count_tokens(char *str)
{
    int count = 0;
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
            while (str[i] && str[i] != quote_type)
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
char    **smart_split(char *str)
{
    int i;
    int k;
    int start;
    int in_quote;
    char quote_type;
    char **tokens;
    int token_count;

    k = 0;
    i = 0;
    start = 0;
    in_quote = 0;
    quote_type = '\0';
    token_count = count_tokens(str);
    tokens = malloc(sizeof(char *) * (token_count + 1));
    if (!tokens)
        return (NULL);
    while (str[i])
    {
        while (str[i] == ' ')
            i++;
        if (!str[i])
            break;
        start = i;
        if (str[i] == '\"')
        {
            in_quote = 1;
            quote_type = str[i];
            i++;
            start = i;
            while (str[i] && str[i] != quote_type)
                i++;
            tokens[k++] = ft_strndup(&str[start], i - start);
            if (str[i] == quote_type && in_quote == 1)
                i++;
            in_quote = 0;
        }
        else if (str[i] == '\'')
        {
            in_quote = 1;
            quote_type = str[i];
            i++;
            start = i;
            while (str[i] && str[i] != quote_type)
                i++;
            tokens[k++] = ft_strndup(&str[start], i - start);
            if (str[i] == quote_type && in_quote == 1)
                i++;
            in_quote = 0;
        }
        else if (str[i] == '>')
        {
            int len = 1;
            if (str[i + 1] == str[i])
                len = 2;
            tokens[k++] = ft_strndup(&str[i], len);
            i += len;
        }
        else if (str[i] == '<')
        {
            int len = 1;
            if (str[i + 1] == str[i])
                len = 2;
            tokens[k++] = ft_strndup(&str[i], len);
            i += len;
        }
        else if (str[i] == '|')
        {
            if (str[i + 1] != str[i])
            {
                tokens[k++] = ft_strndup(&str[i], 1);
            }
            i++;
        }
        else
        {
            while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '\"' &&
                str[i] != '>' && str[i] != '<' && str[i] != '|')
                i++;
            if (i > start)
                tokens[k++] = ft_strndup(&str[start], i - start);
        }
    }
    tokens[k] = NULL;
    // for (int j = 0; tokens[j]; j++)
    //     printf("token[%d]: %s\n", j, tokens[j]);
    return (tokens);
}

// int main()
// {
//     char str[] = "echo hello | more >> out.txt";
//     smart_split(str);
//     int c = count_tokens(str);
//     printf("%d", c);
// }
