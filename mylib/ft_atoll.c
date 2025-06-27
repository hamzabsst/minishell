/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 12:55:43 by hbousset          #+#    #+#             */
/*   Updated: 2025/06/26 16:12:18 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myLib.h"

static char	*skip(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	return ((char *)str + i);
}

static int	check_overflow(long long result, char digit, int sign)
{
	if (sign == 1)
	{
		if (result > LLONG_MAX / 10)
			return (1);
		if (result == LLONG_MAX / 10 && (digit - '0') > LLONG_MAX % 10)
			return (1);
	}
	else
	{
		if (result > -(LLONG_MIN / 10))
			return (1);
		if (result == -(LLONG_MIN / 10) && (digit - '0') > -(LLONG_MIN % 10))
			return (1);
	}
	return (0);
}

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	char		*string;

	result = 0;
	sign = 1;
	string = skip(str);
	if (*string == '-' || *string == '+')
	{
		if (*string++ == '-')
			sign = -1;
	}
	while (*string >= '0' && *string <= '9')
	{
		if (check_overflow(result, *string, sign))
			return (LLONG_MAX + 1ULL);
		result = result * 10 + (*string - '0');
		string++;
	}
	return (result * sign);
}
