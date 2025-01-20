/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcmp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/07 23:52:00 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/10/20 15:35:06 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*x;
	unsigned char	*y;
	size_t			c;

	x = (unsigned char *)s1;
	y = (unsigned char *)s2;
	c = 0;
	if (x == y || n == 0)
		return (0);
	while (c < n)
	{
		if (x[c] != y[c])
			return (x[c] - y[c]);
		c++;
	}
	return (0);
}
