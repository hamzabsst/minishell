/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchaman <abchaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:33:24 by abchaman          #+#    #+#             */
/*   Updated: 2025/04/21 19:18:56 by abchaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
    // bool in_quote = false;
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
            // in_quote = true;
            quote_type = str[i++];
            while (str[i] && str[i] != quote_type)
                i++;  
            if (str[i] == quote_type)
                i++;
            // in_quote = false;
        }
        else
            while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '\"')
                i++;
    }
    return (count);
}
char    **smart_split(char *str)
{
    int i;
    int k;
    int start;
    bool in_quote;
    char quote_type;
    char **tokens;
    int token_count;
    
    k = 0;
    i = 0;
    start = 0;
    in_quote = false;
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
        if (str[i] == '\"' || str[i] == '\'')
        {
            in_quote = true;
            quote_type = str[i];
            i++;
            start = i;
            while (str[i] && str[i] != quote_type)
                i++;
            tokens[k++] = ft_strndup(&str[start], i - start);
            if (str[i] == quote_type)
                i++;
            in_quote = false;
        }
        else
        {
            while (str[i] && str[i] != '\'' && str[i] != '\"')
                i++;
            if (i > start)
                tokens[k++] = ft_strndup(&str[start], i - start);
        }
    }
    tokens[k] = NULL;
    return (tokens);
}

// int main()
// {
//     char str[] = "";
//     int c = count_tokens(str);
//     printf("%d", c);
// }