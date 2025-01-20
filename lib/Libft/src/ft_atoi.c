/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:45:43 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/20 15:33:05 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	result;
	int	c;

	sign = 1;
	result = 0;
	c = 0;
	while (str[c] == ' ' || str[c] == '\t' || str[c] == '\n' || str[c] == '\v'
		|| str[c] == '\r' || str[c] == '\f')
		c++;
	if (str[c] == '-' || str[c] == '+')
	{
		if (str[c] == '-')
			sign *= -1;
		c++;
	}
	while (str[c] >= '0' && str[c] <= '9')
	{
		result = result * 10 + (str[c] - '0');
		c++;
	}
	result = result * sign;
	return (result);
}
