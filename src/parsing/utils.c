/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:33:24 by abchaman          #+#    #+#             */
/*   Updated: 2025/05/23 17:04:36 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *ft_strndup(char *str, size_t len, char skip)
{
	size_t	i;
	char	*results;
    int k;

    k = 0;
	results = (char *) malloc((len + 1) * sizeof(char));
	if (!results)
		return (NULL);
	i = 0;
	while (i < len)
	{
        if(str[i] != skip)
		    results[k++] = str[i];
		i++;
	}
	results[k] = '\0';
	return (results);
}
static int count_tokens(char *str)
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
            while (str[j])
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
char    **smart_split(char *str)
{
    int j ;
    int i;
    int k;
    int start;
    int in_quote;
    char quote_type;
    char **tokens;
    int token_count;
    int count;
    int position;

    position = 0;
    j = 0;
    k = 0;
    i = 0;
    start = 0;
    in_quote = 0;
    quote_type = '\0';
    token_count = count_tokens(str);
    tokens = malloc(sizeof(char *) * (token_count + 1));
    if (!tokens)
        return (NULL);
    while (str[i] == ' ')
        i++;
    if(str[i] == '|')
    {
        printf("syntax error near unexpected token `%c'\n", str[i]);
    }
    while (str[i])
    {
        while (str[i] == ' ')
            i++;
        if (!str[i])
            break;
        start = i;
        if (str[i] == '\"')
        {
            count = 0;
            in_quote = 1;
            quote_type = str[i];
            j = i;
            i++;
            start = i;
            while (str[j] && str[j] != ' ')
            {
                if(str[j] == quote_type)
                {
                    count++;
                    position = j;
                }
                j++;
            }
            while (str[i] && position != i)
                i++;
            if(count % 2 == 1)
            {
                printf("Error : unmatched quotes\n");
                return (NULL);
            }
            else if (str[i] == quote_type && in_quote == 1)
            {
                tokens[k++] = ft_strndup(&str[start], i - start, quote_type);
                i++;
            }
            in_quote = 0;
        }
        else if (str[i] == '\'')
        {
            count = 0;
            in_quote = 1;
            quote_type = str[i];
            j = i;
            i++;
            start = i;
            while (str[j] && str[j] != ' ')
            {
                if(str[j] == quote_type)
                {
                    count++;
                    position = j;
                }
                j++;
            }
            while (str[i] && position != i)
                i++;
            if(count % 2 == 1)
            {
                printf("Error : unmatched quotes\n");
                return (NULL);
            }
            else if (str[i] == quote_type && in_quote == 1)
            {
                tokens[k++] = ft_strndup(&str[start], i - start, quote_type);
                i++;
            }
            in_quote = 0;
        }
        else if (str[i] == '>')
        {
            if (in_quote == 0)
            {
                int len = 1;
                if (str[i + 1] == str[i])
                    len = 2;
                tokens[k++] = ft_strndup(&str[i], len, 0);
                i += len;
            }
        }
        else if (str[i] == '<')
        {
            if (in_quote == 0)
            {
                int len = 1;
                if (str[i + 1] == str[i])
                    len = 2;
                tokens[k++] = ft_strndup(&str[i], len, 0);
                i += len;
            }
        }
        else if (str[i] == '|')
        {
            if (str[i + 1] != str[i])
            {
                tokens[k++] = ft_strndup(&str[i], 1 , 0);
            }
            i++;
        }
        else
        {
            while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '\"' &&
                str[i] != '>' && str[i] != '<' && str[i] != '|')
                i++;
            if (i > start)
                tokens[k++] = ft_strndup(&str[start], i - start, 0);
        }
    }
    tokens[k] = NULL;
    // for (int k = 0; tokens[k]; k++)
    //     printf("token[%d]: %s\n", k, tokens[k]);
    return (tokens);
}

// int main()
// {
//     char str[] = "echo hello | more >> out.txt";
//     smart_split(str);
//     int c = count_tokens(str);
//     printf("%d", c);
// }
