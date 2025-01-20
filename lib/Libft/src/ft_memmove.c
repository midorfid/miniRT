/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/07 17:18:51 by ashevchu      #+#    #+#                 */
/*   Updated: 2022/11/03 18:50:34 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	x;
	char	*dest;
	char	*srcc;

	dest = (char *)dst;
	srcc = (char *)src;
	x = 0;
	if (!dst && !src)
		return (NULL);
	if (srcc < dest)
	{
		while (len--)
		{
			dest[len] = srcc[len];
		}
	}
	else
	{
		while (x < len)
		{
			dest[x] = srcc[x];
			x++;
		}
	}
	return (dst);
}
